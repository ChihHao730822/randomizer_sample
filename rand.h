#ifndef RAND_H_
#define RAND_H_
void showBinary(uint32_t length, uint8_t *p);
void countBit(uint32_t length, uint8_t *p, uint32_t *num0, uint32_t *num1);
/*Create crc8 table.*/
int createTable(void);
/*Create reverse crc8 table.*/
void createTable2(void);
/*randomizer input data.*/
void ran(uint32_t length, uint8_t *dataIn,int page);
/*restore the randomization data to origen data.*/
void restoreData(uint32_t length, uint8_t *dataIn,int page);
#endif
