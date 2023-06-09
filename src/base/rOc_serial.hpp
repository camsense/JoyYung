﻿/*!
\file    serial-O-matic.h
\brief   Header file of the class rOc_serial. This class is used for communication over a serial device.
\author  Philippe Lucidarme (University of Angers)
\version 1.2
\date    28 avril 2011
This Serial library is used to communicate through serial port.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is a licence-free software, it can be used by anyone who try to build a better world.
*/


#ifndef ROC_SERIAL_H
#define ROC_SERIAL_H

// Used for TimeOut operations
//#include <sys/time.h>
// Include for windows
#if defined (_WIN32) || defined( _WIN64)
// Accessing to the serial port under Windows
#include <windows.h>
#include <stdint.h>
#else
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
#include <iostream>
// File control definitions
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif
//ver. 2021-1-19

/*!  \class     rOc_serial
\brief     This class is used for communication over a serial device.
*/
class rOc_serial
{
public:
    rOc_serial();

    // Destructor
    ~rOc_serial();

    int64_t GetTimeStamp();
    // Open a device
    char openDevice (const char *Device, unsigned int Bauds);

    // Close the current device
    void closeDevice();

    char writeChar(const char Byte);

    int readChar(unsigned char *pByte,const unsigned int TimeOut_ms=0);

    int readChars( unsigned char *Buffer, unsigned int MaxNbBytes, const unsigned int TimeOut_ms = 0);
    int readData( unsigned char *Buffer, unsigned int expectedBytes);

    // Empty the received buffer
    void flushReceiver();

    // Return the number of bytes in the received buffer
    int peekReceiver();

    // Change CTR status
    void DTR(bool Status);

    // Change RTS status
    void RTS(bool Status);

    // Get CTS bit
    bool isCTS();

    // Get DTR bit
    bool isDTR();

    // Get CTS bit
    bool isRTS();
private:
    bool mHandlesOpen_;

#if defined (_WIN32) || defined( _WIN64)
    HANDLE hSerial;
    COMMTIMEOUTS timeouts;
#endif
#ifdef __linux__
    int fd;
#endif

};



#endif // ROC_SERIAL_H
