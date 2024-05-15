#include "csapp.h"     // Include custom header file
#include <errno.h>     // Include header file for error handling
#include <string.h>    // Include header file for string operations
#include <sys/types.h> // Include header file for system data types
#include <unistd.h>    // Include header file for POSIX operating system API

// Function to read n bytes from a file descriptor
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) // If interrupted by signal handler, retry
        nread = 0;
      else
        return -1; // Return error if read fails
    } else if (nread == 0)
      break; // Break loop if EOF is reached
    nleft -= nread;
    bufp += nread;
  }
  return (n - nleft); // Return number of bytes read
}

// Function to write n bytes to a file descriptor
ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) // If interrupted by signal handler, retry
        nwritten = 0;
      else
        return -1; // Return error if write fails
    }
    nleft -= nwritten;
    bufp += nwritten;
  }
  return n; // Return number of bytes written
}

// Function to initialize a rio_t structure for buffered reading

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;              // Set file descriptor
  rp->rio_cnt = 0;              // Initialize buffer count
  rp->rio_bufptr = rp->rio_buf; // Set buffer pointer to start of buffer
}

// Internal function to read from a rio_t structure into a buffer

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  int cnt;

  while (rp->rio_cnt <= 0) { // Refill buffer if empty
    
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    
    if (rp->rio_cnt < 0) {
      
      if (errno != EINTR) // If not interrupted by signal handler, return error
        return -1;
      
    } else if (rp->rio_cnt == 0)
      
      return 0; // Return 0 if EOF is reached
      
    else
      rp->rio_bufptr = rp->rio_buf; // Reset buffer pointer
  }
  cnt = n; // Initialize count to number of bytes requested
  
  if (rp->rio_cnt < n) cnt = rp->rio_cnt; // Adjust count if fewer bytes are available
  
  memcpy(usrbuf, rp->rio_bufptr, cnt); // Copy bytes from internal buffer to user buffer
  
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  
  return cnt; // Return number of bytes read
}

// Function to read a line of text from a rio_t structure

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  
  int n, rc;
  char c, *bufp = usrbuf;

  for (n = 1; n < maxlen; n++) {
    
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      
      *bufp++ = c;
      if (c == '\n') {
        
        n++; // Increment count if newline is encountered
        break;
      }
    } else if (rc == 0) {
      if (n == 1)
        return 0; // Return 0 if EOF is reached without reading any data
      else
        break; // Break loop if EOF is reached after reading some data
    } else
      return -1; // Return error if read fails
  }
  *bufp = 0;    // Null-terminate the buffer
  
  return n - 1; // Return number of bytes read
}
