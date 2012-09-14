#ifndef __RAS_UART_H__
#define __RAS_UART_H__

#include <utils/uartstdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Some useful macros
//
//*****************************************************************************
#define InitializeUART()                                                                                \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		\
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	\
        UARTStdioInit(0)                                // Initialize UART, port 0
#define NL              UARTprintf("\r\n")            	// print a newline
#define TAB             UARTprintf("\t")             	// print a tab
#define PRINT(s)        UARTprintf("%s", s)             // print a string
#define PRINT_C(c)      UARTprintf("%s: %c", #c, c)     // print a char variable
#define PRINT_D(d)      UARTprintf("%s: %d", #d, d)     // print a decimal variable
#define PRINT_I(i)      PRINT_D(i)   					// print an integer variable
#define PRINT_S(s)      UARTprintf("%s: %s", #s, s)     // print a string variable
#define PRINT_U(u)      UARTprintf("%s: %u", #u, u)     // print an unsigned variable
#define PRINT_X(x)      UARTprintf("%s: %x", #x, x)     // print a hexadecimal variable

#ifdef UART_BUFFERED
        #define UARTIsCharAvail()       UARTRxBytesAvail()
#else
        #define UARTIsCharAvail()       UARTCharsAvail(UART0_BASE)
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
