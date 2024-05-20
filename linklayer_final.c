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



unsigned char SET[5]={0x5C, 0x03, 0x07, 0x04, 0x5C};
unsigned char UA[5]={0x5C, 0x03, 0x06, 0x05, 0x5C};
int res;

int llopen(linkLayer packet){

    unsigned char buf[255];
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
//Reiniciar o stop
STOP=FALSE;

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
		buf4[0]=0x5c;
		buf4[1]=0x03;
		buf4[2]=0x80;
		buf4[3]=0x03^0x80;
		buf4[j+4]=0x5c;
		for(i=0;i<j;i++){
			buf4[i+4]=buf3[i];
		}		
		return j;
	}

int read(char* buf){
    int state=0;
    int size=strlen(buf);
    unsigned char buf3[256]={};
    
    res = write(fd,buf,5);
    printf("%d bytes written\n", res);
    
    while (STOP==FALSE) {       /* loop for input */
        res = read(fd,buf,1);   /* returns after 1 chars have been input */ 
        if(res>0){
                                /*implementar maquina de estados, quando chega ao ultimo estado, STOP=TRUE*/   
                switch(state){
                    case 0:
                        if(buf[0]==0x5c){
                            state=1;
                        }
                        break;
                    case 1:
                        if(buf[1]==0x03){
                            state=2;
                        }
                        else if(buf[1]==0x5c){
                            state=1;
                        }
                        else{
                            state=0;
                        }
                        break;
                    case 2:
                        if(buf[2]==0x80){
                            state=3;
                        }
                        else if(buf[2]==0x5c){
                            state=1;
                        }
                        else{
                            state=0;
                        }
                        break;
                        //byte destuffing
                    case 3:
                        if(buf[3]==0x03^0x80){
                            for(i=4;i<size;i++){
                                if(buf[i]!=0x5c){
                                    if(buf[i]==0x5d && buf[i+1]==0x7c){
                                        buf3[i]=0x5c;
                                    }
                                    else if(buf[i]==0x5d && buff[i+1]==0x7d){
                                        buf3[i]=0x5d;
                                    }
                                    else{
                                        buf3[i]=buf[i];
                                    }
                                }
                            }
                        }
                        else{
                            state=4;
                        }
                    case 4:
                        if(buf[0]==0x5c){
                           STOP=TRUE;
                        }
                        else{
                            state=0;
                        }
                        break;
                }
        }
    }
    if(STOP==TRUE) return NULL;
}


