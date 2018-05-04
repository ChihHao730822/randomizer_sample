#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc8.h"
#include "rand.h"

uint8_t crcTable[4][256];
uint8_t revTable[4][256]={0};
uint8_t crc8poly[4]={0xd5,0xab,0x9b,0xd9};


void showBinary(uint32_t length, uint8_t *p)
{
    uint32_t i,j,checkBit;
    for(i=0;i<length;i++){
	if(!(i&7))
	    printf("\n");
	checkBit = 0x80;
	for(j=0;j<8;j++){
	    printf("%d",p[i]&checkBit?1:0);
	    checkBit>>=1;
	}
    }
    
    printf("\n");
}

void countBit(uint32_t length, uint8_t *p, uint32_t *num0, uint32_t *num1)
{
    uint32_t i,j,checkBit;

    for(i=0;i<length;i++){
	checkBit = 0x80;
	for(j=0;j<8;j++){
	    if(p[i] & checkBit)
		*num1+=1;
	    else
		*num0+=1;
	checkBit>>=1;
	}
    }
}

int createTable(void){

    int i,j,k;
    for(i=0;i<4;i++){
	for(j=0;j<256;j++)
            crcTable[i][j]=j;
    }
    for(i=0;i<4;i++){
        for(j=0;j<256;j++){
            for(k=0;k<8;k++){
                if(crcTable[i][j] & 0x80){
                    crcTable[i][j] <<= 1;
                    crcTable[i][j] ^= crc8poly[i];
                }else{
                    crcTable[i][j]<<=1;
                }
            }
         }
    }
}
void createTable2(void)
{
    int i,j,k;
    uint8_t te,temp,msb;
    uint8_t loopcnt[4]={177,255,245,243};//shift count to back to origen number
    for(i=0;i<4;i++){
	for(j=1;j<256;j++){
	    temp = crcTable[i][j];	
	    te = crcTable[i][j];
	    for(k=0;k<=loopcnt[i];k++){
        	msb = te & 0x80;
        	te <<= 1;
        	if(msb)
            	te ^= crc8poly[i];
        //	printf("%d, %02x\n",i,te);
	    }
	    revTable[i][temp]=te;
	}
    }
	
}
void ran(uint32_t length, uint8_t *dataIn,int page)
{
    int i;
    int tableNum=page%4;
    //int byteShift = page % 17;
    
    dataIn[0]=crc8[tableNum][dataIn[0]] ^ crc8[tableNum][(page+page%3)&0xff];
   
    for(i=1;i<length;i++)
    {
	dataIn[i] = crc8[(tableNum+i/256)&3][dataIn[i]] ^ crc8[tableNum][(i%256+i+i/256)&0xff];
	
        if((page%8)>3){
	        dataIn[i]^=0xff;
	}
    }
}

void restoreData(uint32_t length, uint8_t *dataIn,int page)
{
    
    int i;
    int tableNum=page%4;
    for(i=1;i<length;i++)
    {
	if((page%8)>3){
	        dataIn[i]^=0xff;
	}
	dataIn[i] = revcrc8[(tableNum+i/256)&3][dataIn[i]^ crc8[tableNum][(i%256+i+i/256)&0xff]];
    }
    dataIn[0]=revcrc8[tableNum][dataIn[0] ^ crc8[tableNum][(page+page%3)&0xff]];
}

