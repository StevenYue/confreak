#include <serial_helper.h>

int openSerial(const char* portName, int mode)
{
    return open(portName, mode);
}

void closeSerial(int fd)
{
    close(fd);
}

int configSerial(int fd)
{
    struct termios SerialPortSettings;	/* Create the structure                          */
    tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
    cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */
    SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
    SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 0; /* Read at least 0 characters */
    SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */
    return tcsetattr(fd,TCSANOW,&SerialPortSettings);
}


int writeSerial(int fd, const char* buf, int bufSize)
{
    return write(fd, buf, bufSize);
}


int readSerial(int fd, char* buf, int bufSize)
{
    return read(fd, buf, bufSize);
}
