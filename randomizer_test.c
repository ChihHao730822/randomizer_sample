#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "rand.h"
#define DATASIZE 100
//#define NOISE

void randomData(uint8_t *data)
{
    int i;
    for(i=0;i<4096;i++){
        data[i]=rand()&0xff;
    }
}

void sameData(uint8_t *data, int loop)
{
    int i;
    for(i=0;i<4096;i+=4){
        if(i<2048){
            data[i]= 0x00;
            data[i+1]= 0x01;
            data[i+2]= 0x00;
            data[i+3]= 0x01;
        }else{
            data[i]= 0x10;
            data[i+1]= 0x00;
            data[i+2]= 0x10;
            data[i+3]= 0x00;
        }    
    }
}

void showData(uint8_t *data, uint32_t length)
{
    int i;
    for(i=0;i<length;i++){
        if(!(i&31))
            printf("\n");
        if(!(i&511))
            printf("\n");
        printf("%02x ",data[i]);   
   }
   printf("\n");
}

void recode_data(FILE *fp,uint8_t *data)
{
    int i;
    for(i=0;i<4096;i++){
	if(!(i&511))
	    fprintf(fp,"\n");
	if(!(i&15))
	    fprintf(fp,"\n");
	fprintf(fp,"%02x ",data[i]);
        }
}

int main()
{
    int i,j;
    uint8_t data[4096];
    uint8_t pFB[DATASIZE];//First Byte per page
    FILE *fp, *fp2,*fp3;
    fp=fopen("origenDataLog.txt","w");
    fp2=fopen("randDataLog.txt","w");
    fp3=fopen("randomizeredData.txt","w");
    
    if(!fp){
        printf("File open fail\n");
        return -1;
    }
    if(!fp2){
        printf("File open2 fail\n");
        return -1;
    }
    if(!fp3){
        printf("File open3 fail\n");
        return -1;
    }
   
    srand(time(NULL));

    int loop;
    int od0[DATASIZE],od1[DATASIZE],
        rd0[DATASIZE],rd1[DATASIZE];
    for(loop=0;loop<DATASIZE;loop++){
        //randomData(data);
        sameData(data,loop);
        countBit(4096,data,&od0[loop],&od1[loop]);
    
#ifdef NOISE 
        //printf("Origen data\n");
        //showData(data,4096);
#endif 
        ran(4096,data,loop);
	//recoed data that had randomziered.
	recode_data(fp3,data);

        pFB[loop]=data[0];
        countBit(4096,data,&rd0[loop],&rd1[loop]);
#ifdef NOISE
        //printf("Random data\n");
        //printf("=============================\n");
        //showData(data,4096);
#endif
        restoreData(4096,data,loop);
#ifdef NOISE
        printf("Restore data\n");
        printf("=============================\n");
        showData(data,4096);
#endif
    }
    showData(pFB,DATASIZE);
    for(loop=0;loop<DATASIZE;loop++){
        fprintf(fp,"%d %d\n",od0[loop],od1[loop]);
        fprintf(fp2,"%d %d\n",rd0[loop],rd1[loop]);
    }
    //for(loop=0;loop<DATASIZE;loop++){
    //    fprintf(fp2,"%d %d\n",rd0[loop],rd1[loop]);
   // }
    //printf("num0=%d, num1=%d\n",num0,num1);
    //fprintf(fp,"%d",15);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
}
