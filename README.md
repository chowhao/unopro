### Introduce

Command line to program 28cxx eeprom by arduino uno
Sometimes, we need to burn file to eeprom, burn the tool like (tl886 ii plus) is expensive, we can make a programmer by ourselves
So, I choose the arduino uno to make a eeprom programmer, this is a command line tool to program 28cxx eeprom chip by arduino uno
It can program the 28c256 eeprom chip right now

#### First Part

##### schematic
The schematic is very simple, it just consists of two 74hc595 chip, you can even build it on breadboard, the detail is as flllows
<img src="https://github.com/2076625923/arduino-programmer/blob/main/sch.png" width="450" height="270">

#### Second Part

##### arduino uno
The receive.ino is for arduino uno, you can download it first, compile the receive.ino and upload to your arduino uno
Then open the serial monitor from arduino ide, the default baudrate is 9600 (you can change in the ino file) 
Type 0000000100 plus enter, you can see some output in serial monitor, 0000000010 means read 0x0010 bytes from orginal address 0x0000
00 is read command, 0000 is orginal address, 0010 is byte number, the address and number should be hex format without prefix
the output is as flolows (read-first-no.png)

<img src="https://github.com/2076625923/arduino-programmer/blob/main/read-first-no.png" width="450" height="220">

##### comamnd
00 --> read,  example: 0000000010 --> read 0x0010 bytes from orginal address 0x0000 (Fig1, read-first-no.png)
01 --> write, example 0100001f2f3f4f --> write 0x1fh, 0x2fh, 0x3fh, 0x4fh from orginaal address 0x0000 (Fig2, read-after-write.png)
0e --> erase, example 0e00000010 --> erase 0x0010 bytes from orginal address 0x0000 (Fig3, read-after-erase.png)

<img src="https://github.com/2076625923/arduino-programmer/blob/main/read-first-no.png" width="270" height="200"><img src="https://github.com/2076625923/arduino-programmer/blob/main/read-after-write.png" width="270" height="200"><img src="https://github.com/2076625923/arduino-programmer/blob/main/read-after-erase.png" width="270" height="200"/>

### Third Part 

#### c program
The unopro.c is a single c file, it work as a command line tool, it get user input and send or read message from arduino 
Althougth, we can program 28c256 from arduino serial monitor, but it can't burn a binary file (such as a.bin) to eeprom.
So I create the unopro.c to read binary file from PC then send data to arduino uno to complete the program progress

#### compile

gcc unopro.c -o unoPro

