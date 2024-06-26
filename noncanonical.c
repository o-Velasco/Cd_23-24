/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define NUM_STATES 5


volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    unsigned char UA[5]= {0x5C, 0x03, 0x06, 0x05, 0x5C};
    unsigned char SET[5]= {0x5C, 0x03, 0x07, 0x04, 0x5C};
      

    if ( (argc < 2) ||
         ((strcmp("/dev/ttyS0", argv[1])!=0) &&
          (strcmp("/dev/ttyS1", argv[1])!=0) )) {
        printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
        exit(1);
    }


    /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
    */


    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(argv[1]); exit(-1); }

    if (tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
        perror("tcgetattr");
        exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */

    /*
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a
    leitura do(s) próximo(s) caracter(es)
    */


    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }
    int state=0;
    int i;

    printf("New termios structure set\n");

    while (STOP==FALSE) {       /* loop for input */
        res = read(fd,buf,5);   /* returns after 5 chars have been input */ 
        printf("dd");
        if(res>0){
                                /*implementar maquina de estados, quando chega ao ultimo estado, STOP=TRUE*/   
            while(state<NUM_STATES){
                switch(state){
                    case 0:
                        if(res==SET[0]){
                            state++;
                            printf("state0\n");
                        }
                        break;
                    case 1:
                        if(res==SET[1]){
                            state++;
                            
                        }
                        break;
                    case 2:
                        if(res==SET[2]){
                            state++;
                        }
                        break;
                    case 3:
                        if(res==SET[3]){
                            state++;
                        }
                        break;
                    case 4:
                        if(res==SET[4]){
                            state++;
                        }
                        break;
                }
            }
           
        STOP=TRUE;
        }
    }


    /*
    O ciclo WHILE deve ser alterado de modo a respeitar o indicado no guião
    */

    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
