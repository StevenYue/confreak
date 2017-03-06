#ifndef INCLUDE_SERIAL_HELPER
#define INCLUDE_SERIAL_HELPER
#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

//given serial port name and mode, return a file descriptor
int openSerial(const char* portName, int mode);

//close the serial port fd
void closeSerial(int fd);

//config serial port with the classic 8N1(8bit, No Parity, 1 stop bic)
//return 0 on success, else as error
int configSerial(int fd);

//write to serial port, return number of bytes written
int writeSerial(int fd, const char* buf, int bufSize);

//read from serial port, return number of bytes read
int readSerial(int fd, char* buf, int bufSize);
#endif
