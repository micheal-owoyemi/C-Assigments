#define RIO_BUFSIZE 8192 // Define the buffer size for rio_t structure

typedef struct {
  int rio_fd;                // File descriptor
  int rio_cnt;               // Number of bytes currently in buffer
  char *rio_bufptr;          // Pointer to next unread byte in buffer
  char rio_buf[RIO_BUFSIZE]; // Buffer to hold data
} rio_t;                     
