#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <linklayer.h>


unsigned char buf[255];
unsigned char SET[5]={0x5C, 0x03, 0x07, 0x04, 0x5C};
unsigned char UA[5]={0x5C, 0x03, 0x06, 0x05, 0x5C};

int llopen(linkLayer packet){
    
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
    if(STOP==TRUE) return 1;
}

int llwrite(char* buf1[],int size){
		unsigned char buf2[256]={};
		unsigned char buf3[513]={};


		int i=0, j=0;
		unsigned char xor_result=0;

		for(i=0;i<256;i++){
			buf2[i]=buf1[i];
			xor_result ^=  buf1[i];
		}
		buf2[256]=xor_result;

		for(i=0;i<257;i++){
			if (buf2[i]==0x5c){
				buf3[j]=0x5d;
				buf3[j+1]=0x7c;
			}
			else if(buf2[i]==0x5d){
				buf3[j]=buf2[i];
				buf3[j+1]=0x7d;
			}
			else{
				buf3[j]=buf2[i];
			}
			j++;
		}
		//trama I
		unsigned char buf4[j+5]={};
		buf4[0]=SET[0];
		buf4[1]=SET[1];
		buf4[2]=SET[2];
		buf4[3]=SET[3];
		buf4[j+4]=SET[4];
		for(i=0;i<j;i++){
			buf4[i+4]=buf3[i];
		}		
		return j;
	}
