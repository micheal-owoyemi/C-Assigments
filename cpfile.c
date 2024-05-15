#include "csapp.h"
#include <unistd.h>

// Function declarations
void rio_readinitb(rio_t *rp, int fd);       // Initialize rio_t structure for reading
void rio_writen(int fd, void *usrbuf, size_t n); // Write n bytes from usrbuf to file descriptor fd
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen); // Read a line of text from rio_t buffer

int main(int argc, char **argv) {
  int n;
  rio_t rio;    // Rio_t structure for buffered input/output
  char buf[RIO_BUFSIZE]; // Buffer to store data read from standard input

  // Initialize rio_t structure for standard input
  rio_readinitb(&rio, STDIN_FILENO);

  // Read lines of text from standard input and write them to standard output
  while ((n = rio_readlineb(&rio, buf, RIO_BUFSIZE)) != 0) {
    rio_writen(STDOUT_FILENO, buf, n); // Write the read line to standard output
  }

  return 0;
}
