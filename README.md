# arduino-programmer

Command line to program 28cxx eeprom by arduino uno

This is a command line tool to program 28cxx eeprom chip by arduino uno

It can program the 28c256 eeprom chip rightly now

## arduino uno
The receive.ino is for arduino uno, you can download it first, compile the receive.ino and upload to your arduino uno 

Then open the serial monitor, baudrate is 9600(you can change in the ino file) 

Type 0000000100 plus enter, you can see some output, the command 0000000100 means read 0x0100 bytes from orginal address 0x0000

00 is read command 0000 is orginal address 0100 is byte number, the address and number should be hex format without prefix

![image](https://github.com/2076625923/arduino-programmer/blob/main/dump.png)

### comamnd line 
There are three command in all:

00 --> read,  example: 0001000010 --> read 0x0010 bytes from orginal address 0x0100

01 --> write

0e --> erase
