### Introduce

A command line to program 28cxx eeprom by arduino uno

Sometimes, we need to burn file to eeprom, burn the tool like (tl886 ii plus) is expensive, we can make a programmer by ourselves

So, I choose the arduino uno to make a eeprom programmer, this is a command line tool to program 28cxx eeprom chip by arduino uno

It can program the 28c256 eeprom chip right now !!!

#### First Part (Hardware)

##### schematic

The schematic is very simple, it just consists of two 74hc595 chip, you can even build it on breadboard, the detail is as flllows

<img src="https://github.com/2076625923/arduino-programmer/blob/main/sch.png" width="400" height="200">

#### Second Part (Software)

##### arduino program

The receive.ino is a file for arduino uno, compile it and upload to your arduino uno, the open serial monitor at 9600 baurate

Type 0000000020 and send, you can see some output in monitor,  00 is the read command,  0000 is orginal address,  0010 is byte number

It means read 0x0020 bytes from orginal address 0x0000, the output is as flolows (read-first-no.png)

<img src="https://github.com/2076625923/arduino-programmer/blob/main/read-first-no.png" width="450" height="220">

There are two other command write and erase, the erase and read command is 10 chars without '\n', the write command is more than 8 chars

0100001f2f3f4f --> write 0x1fh, 0x2fh, 0x3fh, 0x4fh from orginal address 0x0000 to eeprom, the result is read-after-write.png

0e00000010 --> erase 0x0010 bytes from orginal address 0x0000, the result is read-after-erase.png

<img src="https://github.com/2076625923/arduino-programmer/blob/main/read-after-write.png" width="400" height="200">                                       <img src="https://github.com/2076625923/arduino-programmer/blob/main/read-after-erase.png" width="400" height="200"/>

##### c program

Based on the arduino program, we can write,read,erase some bytes in 28c256 by arduino uno, But it can't work efficiencily

For example, we have to write bytes mannualy, we can't burn a binary file to 28c256 by arduino program

So I create the unopro.c to read binary file from PC then send data to arduino uno to complete the program progress

###### Step1:compile

gcc unopro.c -o unoPro

###### Step2:connect arduino

unoPro   9600   /dev/ttyACM0

###### Step3:usage

The usage is shown in follow picture

<img src="https://github.com/2076625923/arduino-programmer/blob/main/usage.png" width="400" height="600">  <img src="https://github.com/2076625923/arduino-programmer/blob/main/final.png" width="400" height="600">

### Bugs

After erase and load command, there is some error the first line in dump command output

### Update

Now, I have fix above bugs it can work perfectly, as shown in final picture

### Reference

https://github.com/beneater/eeprom-programmer
