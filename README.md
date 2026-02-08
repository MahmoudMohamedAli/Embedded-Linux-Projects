# Embedded-Linux-Projects
---

### [Blink_Led](Blink_Led/)
- the Led on physical pin 16 "gpio 23" is toggling every half second.
- i used sysfs directly and i don't rely on external lib like "gpiod".
- [you can see from logic analyzer pic the loggling sequence.](Blink_Led/2.png) 
- [output.](Blink_Led/1.png)
- [use Gpiod lib source code.](Blink_Led/Blink_Led_Gpiod.cpp)
- [Write the BlinkLed app in python.](Blink_Led/Blink_Led_Gpiod.py)

### PWM_App
 - [in this app i used Hardware PWM " PWM0 " module physical pin number "18,12" these two pins are wired to same PWM module.](PWM/Led_Control_PWM_Hardware.cpp)
 - [i used as well gpiod  lib to implement PWM concept on gpio pins. physical pin number "16" on board.](PWM/Led_Control_gpio.cpp)
 - [i wrote service that start app at boot of Linux](PWM/pwm-led.service)

### UART_App
 - [communicate with Raspi over Uart protocol](UART_APP/uart_comm.cpp)
 - [Uart App that uses two different thread one for Rx and other for Tx](UART_APP/uart_TXWorker_RXWorker.cpp).
 - [this App control led behavior vai UART "Turn on / turn off / Toggle.](UART_APP/Control_Led_Via_UART.cpp).
 - the serial app that use to send Data i created it using QT framework.
 - [Here is the link of the source code of Serial app using Qt v6 Framework](https://github.com/MahmoudMohamedAli/Serial_App_QT/tree/main/Version1)
 - [Image that show the out of controlling Led](UART_APP/1.png) 

### [Qt integration with RPI]
-