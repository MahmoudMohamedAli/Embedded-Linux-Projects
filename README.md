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
 -[communicate with Raspi over Uart protocol](UART_APP/uart_comm.cpp).
 
### [Qt integration with RPI]
-