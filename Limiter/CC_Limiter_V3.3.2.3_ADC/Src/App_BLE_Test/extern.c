/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : extern.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/05/2017
*
* Description        : extern 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "includes.h"

/* Public typedef ------------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

const char Model_Name[15] = "LDT-300BS";
const char Model_Name_KT[15] = "LDT-300BS-ADAS";
const char PCB_HW_Ver[10] = "REV 1.00";
const char Approval_No[12] = "18-0001-00";
char Fota_FW_Ver[10];
char IP[4];

const char FW_Ver_Key[] = {'L','O','O','P',MAJOR+'L', MINOR+'O', RELEASE+'O', BUILD+'P', CDMA_TYPE};

uint32_t _millisCounter;

const unsigned short crc16tab[256]= {
0x0000,0x1021,0x2042,0x3063,0x4084,
0x50a5,0x60c6,0x70e7,0x8108,0x9129,
0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,
0xf1ef,0x1231,0x0210,0x3273,0x2252,
0x52b5,0x4294,0x72f7,0x62d6,0x9339,
0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,
0xf3ff,0xe3de,0x2462,0x3443,0x0420,
0x1401,0x64e6,0x74c7,0x44a4,0x5485,
0xa56a,0xb54b,0x8528,0x9509,0xe5ee,
0xf5cf,0xc5ac,0xd58d,0x3653,0x2672,
0x1611,0x0630,0x76d7,0x66f6,0x5695,
0x46b4,0xb75b,0xa77a,0x9719,0x8738,
0xf7df,0xe7fe,0xd79d,0xc7bc,0x48c4,
0x58e5,0x6886,0x78a7,0x0840,0x1861,
0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,
0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,
0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc,
0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,
0xab1a,0x6ca6,0x7c87,0x4ce4,0x5cc5,
0x2c22,0x3c03,0x0c60,0x1c41,0xedae,
0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,
0x8d68,0x9d49,0x7e97,0x6eb6,0x5ed5,
0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,
0xaf3a,0x9f59,0x8f78,0x9188,0x81a9,
0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,
0xe16f,0x1080,0x00a1,0x30c2,0x20e3,
0x5004,0x4025,0x7046,0x6067,0x83b9,
0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,
0xe37f,0xf35e,0x02b1,0x1290,0x22f3,
0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,
0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,
0x14a0,0x0481,0x7466,0x6447,0x5424,
0x4405,0xa7db,0xb7fa,0x8799,0x97b8,
0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,
0x36f2,0x0691,0x16b0,0x6657,0x7676,
0x4615,0x5634,0xd94c,0xc96d,0xf90e,
0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
0x5844,0x4865,0x7806,0x6827,0x18c0,
0x08e1,0x3882,0x28a3,0xcb7d,0xdb5c,
0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,
0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,
0x0af1,0x1ad0,0x2ab3,0x3a92,0xfd2e,
0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,
0x9de8,0x8dc9,0x7c26,0x6c07,0x5c64,
0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,
0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,
0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,
0x1ef0
};

/* Public function prototypes ------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

void Mem_Copy(u8 *dest, u8 *source, u16 len)
{
  u16 i;
  
  for(i = 0; i < len; i++) 
    *dest++ = *source++;    
}
void Mem_Copy2(u8 *dest, u8 *source, u16 len)
{
  u16 i;
  u8  tmp;
  
  for(i = 0; i < len; i++) {
    tmp = *source++; 
    if(tmp == 0x00 || tmp == 0xFF) tmp = ' ';
    *dest++ = tmp;   
  }
}
void Val_Copy(u8 *dest, u32 source, u8 len)
{
  u8 i;
  
  for(i = 0; i < len; i++)
    *dest++ = (u8)(source>>(8*i));    
}


unsigned short int  getcrc16(u16 crc, unsigned char *buf, unsigned int len)
{
//  volatile  unsigned int counter;
//  volatile  unsigned short int crc=0;
  unsigned int counter;

  
  for( counter = 0; counter < len; counter++)
  {
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *buf++)&0x00FF];
  }
  return(crc);
}

unsigned short int  getcrc16b(unsigned char *buf, unsigned int len)
{
//  volatile  unsigned int counter;
//  volatile  unsigned short int crc=0;
  u16 crc=0;
  unsigned int counter;

  
  for( counter = 0; counter < len; counter++)
  {
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *buf++)&0x00FF];
  }
  return(crc);
}

unsigned short getcrc16c(unsigned short crc, unsigned char data)
{
  return((crc<<8) ^ crc16tab[((crc>>8) ^ data)&0x00FF]);
}

u8  IsSameDay(u32 time1, u32 time2)
{

  if((time1/86400) == (time2/86400))
    return 1;
  else
    return 0;
}

u32  CalDayCnt(u32 time1, u32 time2)
{
  u32 day_cnt = 0;
  u32 d1, d2;
  
  if(time1 > time2) {
    d1 = time1-(time1%86400); 
    d2 = time2-(time2%86400); 
  }
  else {
    d1 = time2-(time2%86400); 
    d2 = time1-(time1%86400); 
  }
  day_cnt = (d1-d2)/86400;
  return day_cnt;
}


long SwapEndian32 (u32 n)
{
return((n&0xff)<<24)+((n&0xff00)<<8)+((n&0xff0000)>>8)+((n>>24)&0xff);
}

u16 SwapEndian16 (u16 n)
{
return ((n>>8)&0xff)+((n<<8)&0xff00);
}


/*
********************************************************************************
* Description : StringLen
* Arguments   : str1, len
* Return      : length
* Note        : Compare string
******************************************************************************** 
*/
int StringLen(char* str, int len)
{  
  for(int i = 0; i < len; i++) {
    if(str[i] == 0)
      return i;
  }

  return len;
}

/*
********************************************************************************
* Description : IsSameString
* Arguments   : str1, str2, len
* Return      : 
* Note        : Compare string
******************************************************************************** 
*/
u8 IsSameString(char* str1, char* str2, u8 len)
{  
  for(int i = 0; i < len; i++) {
    if(str1[i] != str2[i])
      return 0;
  }
  
  return 1;
}

/*
********************************************************************************
* Description : IsSameString2
* Arguments   : str1, str2, mlen, len, start
* Return      : 
* Note        : Compare string2
******************************************************************************** 
*/
u8 IsSameString2(char* str1, char* str2, u8 mlen, u8 len, u8* start)
{ 
  u8 idx = 0;
  
  for(int i = 0; i < mlen; i++) {
    if(str1[i] == str2[idx]) {
      idx++;
      if(idx == len) {
        *start = i+1;
        return 1;
      }
    }
    else idx = 0;
  }
  
  return 0;
}

/*
********************************************************************************
* Description : millis
* Arguments   : none
* Return      : 
* Note        : miliis time 
******************************************************************************** 
*/
uint32_t millis(void) 
{ 
 	return _millisCounter; 
}

void init_millis(void) 
{ 
 	_millisCounter = 0; 
}


/*
********************************************************************************
* Description : AsciiToVal
* Arguments   : str
* Return      : val
* Note        : AsciiToVal
******************************************************************************** 
*/
long AsciiToVal(char* str)  
{
  long val = 0;
  int i, sign = 1;
  
  if(str[0] == '-') {
    sign = -1;
    for(i = 0; i < 9; i++) {
      str[i] = str[i+1];
    }
  }
  for(i = 0; i < 10; i++) {
    if(str[i] >= 0x30 && str[i] <= 0x39) {
      if(i > 0) val *= 10;
      val += str[i]-0x30;
    }
    else if(str[i] == 0) break;
    else return -1;
  }
  return (sign*val);
}

/*
********************************************************************************
* Description : IpToInt
* Arguments   : str
* Return      : val
* Note        : Ip to Integer
******************************************************************************** 
*/
char IpToInt (const char * ip)
{
    /* The return value. */
    //unsigned v = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    const char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                return 0;
            }
        }
        if (n >= 256) {
            return 0;
        }
        IP[i] = n;
        //v *= 256;
        //v += n;
    }
    return 1;
}
/******************* (C) COPYRIGHT 2017 LOOP  ******************END OF FILE****/