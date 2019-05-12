To wire the car you should connect:
 1. The SCL pin from the PWM breakout board to pin 117 from the Gumstix Board.
 2. The SDA pin from the PWM breakout board to pin 118 from the Gumstix Board.
 3. The VCC and GND pins from the PWM breakout board to the VCC and GND pins from the 	  Gumstix Board.
 4. Connect the power supply for the PWM channel to the 5 volt supply from Arduino.
 5. Connect channel 0 to ESC from the car.
 6. Connect channel 1 to Servo controller.  
 7. Connect the VCC and GND from IR range finder to the 5 volt supply from Arduino.
 8. Connect signal line from IR range to analog pin A0.
 9. Connect Arduino pin 2 to LED and Gumstix pin 101.
 10. Connect the Chronos Watch access point to the Gumstix Board.
 11. Connect the Arduino and Gumstix to the 5 volt power supply.
 12.  Connect the ESC to the 7.2 volt battery.


In order to run the code you have to compile the user level program and the kernel module. Then you should compile and transfer the Arduino code into the Arduino.
When you start the Gumstix Board transfer both the user level program and the kernel module into the Gumstix and run the following commands:

mknod /dev/mymodule c 61 0
insmod mymodule.ko
./car_control

At the same time you run car_control you should turn on the switch for the 7.2 volt battery (the battery that powers the ESC). After you should use the bottom left button in the Chronos watch to set it to ACC mode. After putting it in ACC mode you should press the bottom right button of the watch to start transmiting data to the access point and control the car.

