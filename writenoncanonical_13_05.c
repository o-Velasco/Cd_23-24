/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    int i, sum = 0, speed = 0;
    /*está no llopen*/
    unsigned char buf[255];
    unsigned char SET[5]={0x5C, 0x03, 0x07, 0x04, 0x5C};
    unsigned char UA[5]={0x5C, 0x03, 0x06, 0x05, 0x5C};
	/*deixa de estar no llopen*/
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

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
        perror("tcgetattr");
        exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 1;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */



    /*
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a
    leitura do(s) próximo(s) caracter(es)
    */


    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }

    printf("New termios structure set\n");

	/*esta no llopen*/
	int state=0;
  

    res = write(fd,SET,5);
    printf("%d bytes written\n", res);
    
    while (STOP==FALSE) {       /* loop for input */
        res = read(fd,buf,1);   /* returns after 1 chars have been input */ 
        if(res>0){
                                /*implementar maquina de estados, quando chega ao ultimo estado, STOP=TRUE*/   
                switch(state){
                    case 0:
                        if(buf[0]==UA[0]){
                            state=1;
                            printf("state1 %d\n", state);
                        }
                        break;
                    case 1:
                        if(buf[0]==UA[1]){
                            state=2;
                            printf("state2 %d\n", state);
                        }
                        else if(buf[0]==UA[0]){
                            state=1;
                            printf("state3 %d\n", state); 
                        }
                        else{
                            state=0;
                            printf("state4 %d\n", state);
                        }
                        break;
                    case 2:
                        if(buf[0]==UA[2]){
                            state=3;
                            printf("state5 %d\n", state);
                        }
                        else if(buf[0]==UA[0]){
                            state=1;
                            printf("state6 %d\n", state);
                        }
                        else{
                            state=0;
                            printf("state7 %d\n", state);
                        }
                        break;
                    case 3:
                        if(buf[0]==UA[3]){
                            state=4;
                            printf("state8 %d\n", state);
                        }
                        else if(buf[0]==UA[0]){
                            state=1;
                            printf("state9 %d\n", state);
                        }
                        else{
                            state=0;
                            printf("state10 %d\n", state);
                        }
                        break;
                    case 4:
                        if(buf[0]==UA[4]){
                           STOP=TRUE;
                            printf("state11 %d\n", state);
                        }
                        else{
                            state=0;
                            printf("state13 %d\n", state);
                        }
                        break;
                }
           
        }
    }
    
    


    /*
    O ciclo FOR e as instruções seguintes devem ser alterados de modo a respeitar
    o indicado no guião
    */

	/*deixa de estar no llopen*/
	sleep(1);
	
	
    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
        perror("tcsetattr");
        exit(-1);
    }


    close(fd);
    return 0;
}
