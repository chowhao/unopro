/*###########################################################
### File Name: unpro.c
### Author: Chowhao
### Mail: 19211448@bjtu.edu.cn
### Created Time: 2023-01-15
###########################################################*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>

#define SIZE 0x8000


int main(int argc, char *argv[])
{
  /* open port and get fd */
  if(argc != 3) 
  {
    printf(
      "Command line tool for unoPro !\n"
      "Usage: unoPro [baud] [port] !\n"
      "Optional BaudRate is 9600, 19200 and 38400 !\n"
    );
    return 1;
  }

  /* port and baud */
  char *port = argv[2];
  char *baud = argv[1];
  

  long int speed = strtol(baud, NULL, 0);	
  if(!(speed == 9600 || speed == 19200 || speed == 38400))
  {
    printf("\033[1;33mBaudRate %s is not available !\033[0m\n", baud);
    printf("Optional BaudRate is 9600, 19200 and 38400 !\n");
    return 1;
  }
  
  int fd = plug(speed, port);

  if(fd == -1)
  { /* can't open port */
    printf("\033[1;31mCan't open %s !\033[0m\n", port);
    return 1;
  }
  /* success open port */
  else  printf("Success open %s, BaudRate is %ld !\n", port, speed);


  /* command */
  char cmd[32];
  char *ptr = cmd;
  char *optv[] = { "help", "dump", "load", "erase", "write", "demo", "clear", "list"};

  /* argument */
  char arg[16][16];
  
  /* others */
  char *ps1 = ">>> ";
  char *exit = "quit";

  /* get command and argument */ 
  printf("Please type 'help' for usage\n");
  printf("%s", ps1);
  gets(cmd); 


  /* continue until quit command */
  while(strcmp(cmd, exit) != 0)
  { 
    /* get cmd char num */
    int num = strchr(cmd, '\0')-cmd;

    /* nothing input*/ 
    if(num == 0)
    {
      printf
      (
        "\033[1;33mNothing input at all !\033[0m\n"
        "Please type 'help' or 'demo' for help !\n"
      );
    }
    else
    { /* something input */
      char seps[] = " ";
      int index = 0;

      /* split cmd to get command and argument */
      char *cli = strtok_r(cmd, seps, &ptr);
      while(cli != NULL)
      {
        strcpy(arg[index], cli);
        index++;
        cli = strtok_r(NULL, seps, &ptr);
      }

      if(index != 0)
      { 
        /* get cmd flag */
        int flag = -1;
        flag = getcmd(optv, arg[0]);

        if(flag != -1)
        { 
          if(flag == 0)
          { /* help command */
            if(index == 1) usage();
            else printf("Do you mean 'help' ?\n");
          }
          else if(flag == 5)
          {
            if(index == 1) warn();
            else printf("Do you mean 'demo' ?\n");
          }
          else if(flag == 6)
          {
            if(index == 1) system("clear");
            else printf("Do you mean 'clear' ?\n");
          }
          else if(flag == 7)
          {
            if(index == 1) system("ls");
            else printf("Do you mean 'list' ?\n");
          }
          else
          {
            if(index == 3)
            { 
              /* handle operation */
              if((strchr(arg[1], '\0')-arg[1]) != 4)
              { /* address error */
                printf("\033[1;33mAddress must be two bytes without prefix !\033[0m\n");
                printf("Your address: %s !\033[0m\n", arg[1]);
                printf("Type 'demo' for usage examples\n");
              }
              else operation(fd, flag, arg);
            }
            else
            { /* argument error */
              printf("Do you mean '%s' ?\n", optv[flag]);
              printf("Type 'demo' for usage examples\n");
            }

          }

        }
        else
        { /* unknown command */
          printf("\033[1;33m%s ?\033[0m\n", arg[0]);
          printf("Please type 'help' for usage\n");
        }

        flag = -1;
      }

      index = 0;

    }

    /* get command line again */      
    printf("%s", ps1);
    gets(cmd); 

  }
  
  /* close */
  close(fd);
  return 0;

}


/* initial serial port */
int plug(long int baud, char *port)
{
  /* get port fd */
  int fd = open(port, O_RDWR);

  if(fd != -1)
  { 
    /* get port settings */
    struct termios ttyopt;
    tcgetattr(fd, &ttyopt);

    /* change port settings */
    ttyopt.c_cflag = 2237;
    ttyopt.c_iflag = 0;
    ttyopt.c_lflag = 0;
    ttyopt.c_oflag = 0;
    ttyopt.c_ospeed = 13;
    ttyopt.c_ispeed = 13;
    ttyopt.c_line = 0;
    
    /* baud setting */
    long int speed[] = {9600, 19200, 38400};
    int rate[] = {B9600, B19200, B38400};
    for(int j=0; j<3; j++)
    {
      if(baud == speed[j]) cfsetispeed(&ttyopt, rate[j]);
    }

    /* commit port settings */
    tcsetattr(fd, TCSANOW, &ttyopt);
  }
  return fd;
}


/* convert hex to int */
int hex2int(char *hex)
{
  int sum = 0;
  int num = strchr(hex, '\0')-hex;
	for(int j=0; j<num; j++)
	{
    if (hex[j] >= '0' && hex[j] <= '9') sum = 0;
    else if (hex[j] >= 'a' && hex[j] <= 'f') sum = 0;
    else if (hex[j] >= 'A' && hex[j] <= 'F') sum = 0; 
    else return -1;
	}
	
  char str[] = "0xffff";
  for(int j=0; j<4; j++) str[j+2] = hex[j];
  sum = strtol(str, NULL, 0);	
	return sum;
}


/* get option from input */
int getcmd(char *opts[], char *opt)
{
  for(int i=0; i<8; i++)
  {
    /* return operation index */
    if(strcmp(opt, opts[i]) == 0) return i;
  }
  /* unknown operation */
  return -1;
}


/* handle operation */
int operation(int fd, int flag, char arg[16][16])
{
  /* address and num */
  int org = hex2int(arg[1]);
  int num = hex2int(arg[2]);
  int all = strchr(arg[2], '\0')-arg[2];

  if(org == -1 || (flag != 2 && num == -1))
  {
    printf("\033[1;33mAddress and size should be hex without prefix !\033[0m\n");
    printf("Please specify address and bytes again !\n");
    return 1;
  }
  
  /* dump command */
  if(flag == 1)
  {
    int size = (num%16 != 0 ? (num/16 + 1)*16 : num);
    if(all == 4 && size != 0 && size+org < SIZE+1) dump(fd, arg[1], arg[2]);
    else
    {
      if(size+org > SIZE) printf("\033[1;33m%sh + %sh > 8000h !\033[0m\n", arg[1], arg[2]);
      else printf("\033[1;33mAddress:%s, Size:%s !\033[0m\n", arg[1], arg[2]);
      printf("Please specify address and size again !\n");
    }
  }
  /* load command */
  else if(flag == 2) 
  {
    FILE *file = fopen(arg[2], "rb");
    if(!file) printf("\033[1;33mFail to open %s !\033[0m\n", arg[2]);
    else
    {
      fseek(file, SEEK_SET, SEEK_END);
      int len  = ftell(file);

      if(len+org > SIZE)
      {
        printf("\033[1;33m%sh + %dd > 8000h !\033[0m\n", arg[1], len);
        printf("Please specify address and file again !\n");
      }
      else load(fd, arg[1], file);
      /* close file */
      fclose(file);
    }
  }
  /* erase command */
  else if(flag == 3)
  {
    if(all == 4 && num != 0 && num+org < SIZE+1) erase(fd, arg[1], arg[2]);
    else
    {
      if(num+org > SIZE) printf("\033[1;33m%sh + %sh > 8000h !\033[0m\n", arg[1], arg[2]);
      else printf("\033[1;33mAddress:%s, Size:%s !\033[0m\n", arg[1], arg[2]);
      printf("Please specify address and size again !\n");
    }
  }
  /* write command */
  else if(flag == 4) 
  {
    int half = all/2;

    if(all%2 == 0 && half != 0 && half+org < SIZE+1)
    {
      mywrite(fd, arg[1], arg[2]);

      /* sleep for wrtite */
      for(int current=0; current<half; current++)
      {
      usleep(20*1000);
      printf("Total: %d Bytes, Burned: %d Bytes\r", half, current+1);
		  fflush(stdout);
      }
      printf("\n");
    }
    else
    {
      if(half+org > SIZE) printf("\033[1;33m%sh + %dd > 8000h !\033[0m\n", arg[1], half);
      else printf("\033[1;33mAddress:%s, Data:%s !\033[0m\n", arg[1], arg[2]);
      if(all%2 != 0) printf("The data should be Even, not Odd!\n");
      printf("Please specify address and size again !\n");
    }
  }
  /* argument error */     
  else
  {
    printf("\033[1;33mArgument specify error !\033[0m\n");
    warn(); 
  }
  return 0;          
}


/* operation right but argument error */
void warn(void)
{
  printf(
    "Examples:\n"
    "dump 0100 02ff    Read 0x02ff bytes from 0x0100\n"
    "load 01ff a.bin   Write file a.bin  from 0x01ff\n"
    "write 01f0 2ff635 Write 2fh,f6h,35h from 0x01f0\n"
    "erase 02f0 0100   Erase 0x0100 bytes from 0x02f0\n");
}


/* print help page */
void usage(void)
{
  printf
  (
    "dump   [org]  [size]     Read some bytes from address\n"
    "load   [org]  [file]     Write binary file to address\n"  
    "write  [org]  [data]     Write some bytes to address\n"  
    "erase  [org]  [size]     Erase some bytes from address\n"
    "clear                    Clear out the screen !\n"
    "list                     List all the files in pwd !\n"
    "help                     Display this help page !\n"
    "demo                     Display some usage examples\n"
    "quit                     Exit from this command tool\n"
  );
}


/* read some bytes from device */
void dump(int fd, char *org, char *size)
{
  char msg[128];
  char buf[256];
  /* specify read operation */
  msg[0] = '0';
  msg[1] = '0';
  msg[10] = '\n';

  /* specify address and size */
  for(int i=0; i<4; i++)
  {
    msg[2+i] = org[i];
    msg[6+i] = size[i];
  }
  
  /* send message to device */
  write(fd, msg, 11);

  /* read some bytes */
  int num = read(fd, buf, sizeof(buf));
  int flag = getout(buf);

  while(num > 0 && flag == 0)
  {
    printf("%s", buf);
    for(int i=0; i<256; i++) buf[i] = '\0';
    num = read(fd, buf, sizeof(buf));
    flag = getout(buf);
  }

  printf("%s", buf);
  printf("\n");
  
  /* flush serial buffer */
  tcflush(fd, TCIFLUSH);
  for(int i=0; i<256; i++) buf[i] = '\0';
  
}


/* exit from loop after read 'Done! */
int getout(char *str)
{
	for(int j=0; j<4; j++)
	{
    if (str[j] == '!') return -1;
	}
	return 0;
}


/* erase some bytes in device */
void erase(int fd, char *org, char *size)
{
  char msg[128];
  /* specify erase operation */
  msg[0] = '0';
  msg[1] = 'e';
  msg[10] = '\n';

  /* specify address and size */
  for(int i=0; i<4; i++)
  {
    msg[2+i] = org[i];
    msg[6+i] = size[i];
  }
  
  /* send message to device */
  write(fd, msg, 11);

  /* sleep for wrtite */
  int sum = hex2int(size);

  for(int current=0; current<sum; current++)
  {
    usleep(20*1000);
    printf("Total: %d Bytes, Erased: %d Bytes\r", sum, current+1);
		fflush(stdout);
  }
  printf("\n");

}


/* write some bytes to device */
void mywrite(int fd, char *org, char *data)
{
  char msg[128];
  /* specify write operation */
  msg[0] = '0';
  msg[1] = '1';
  int num = strchr(data, '\0')-data;
  msg[6+num] = '\n';
  
  /* specify address */
  for(int i=0; i<4; i++)
  {
    msg[2+i] = org[i];
  }

  /* specify data */
  for(int i=0; i<num; i++)
  {
    msg[6+i] = data[i];
  }

  /* send message to device */
  write(fd, msg, num+7);

}


/* write binary file to device */
void load(int fd, char *org, FILE *file)
{
  /* get file size */
  fseek(file, SEEK_SET, SEEK_END);
  int len  = ftell(file);

	/* address and data */
  int ch;
  int sum = 0;
  char tmp[4];
  char data[96];

  /* integer address */
  int address = hex2int(org);

  /* seek file beginning */
	fseek(file, SEEK_SET, SEEK_SET );
  
  for(int j=0; j<len; j += 48)
	{
		for(int k=0; k<48; k++)
		{
      if((ch = fgetc(file)) != EOF)
			{
				sprintf(tmp, "%02x", ch);
				data[2*k] = tmp[0];
				data[2*k+1] = tmp[1];
        sum += 1;
			}
		}

		/* specify address */
		sprintf(org, "%04x", j + address);

		/* write some bytes */
    mywrite(fd, org, data);

		/* clear data */
		for(int j=0; j< 96; j++)
		{
			data[j] = '\0';
		}

    /* sleep for wrtite */
    for(int current=0; current<sum; current++)
    {
      usleep(20*1000);
      printf("Total: %d Bytes, Burned: %d Bytes\r", len, current+j+1);
		  fflush(stdout);
    }
    sum = 0;

	}
  printf("\n");

}
