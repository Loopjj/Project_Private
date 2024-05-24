/*
*************************************** (C) COPYRIGHT 2015 Loop *************************************
*
* File Name          : key.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 06/18/2015
*
* Description        : key process
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define KEY_GLOBALS
#include "includes.h"

#define ENTER   0x01
#define MODE    0x02
#define UP      0x03
#define RESET   0x04
#define HMODE   0x05
#define CMODE   0x06
#define LMODE   0x07
#define NOKEY   0x00

#define BOUNCE	25

//////// Font ////////////////////////////// ///////////////////////
const INT8U ch_num[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};
const INT8U ch_num1[]={0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};

const INT8U ch_A=0x77;
const INT8U ch_B=0x7C;
const INT8U ch_C=0x39;
const INT8U ch_D=0x5E;
const INT8U ch_E=0x79;
const INT8U ch_F=0x71;
const INT8U ch_G=0x3D;
const INT8U ch_H=0x74;
const INT8U ch_I=0x30;
const INT8U ch_J=0x1E;
const INT8U ch_K=0x75;
const INT8U ch_L=0x38;
const INT8U ch_M=0x15;
const INT8U ch_N=0x37;
const INT8U ch_O=0x3F;
const INT8U ch_P=0x73;
const INT8U ch_Q=0x67;
const INT8U ch_R=0x50;
const INT8U ch_S=0x6D;
const INT8U ch_T=0x78;
const INT8U ch_U=0x3E;
const INT8U ch_V=0x3E;
const INT8U ch_W=0x2A;
const INT8U ch_X=0x76;
const INT8U ch_Y=0x6E;
const INT8U ch_Z=0x5B;
const INT8U ch_ba=0x40;
const INT8U ch_qe=0x53;
const INT8U ch_cl=0x00;

INT8U DVal[5];					// 5digit display value
void Display_Parm(void);
void DispVal(unsigned short val);
void Start_Download(void);
void Set_Management(void);
/*
*********************************************************************************************************
*                                               CONSTANT
*********************************************************************************************************
*/


/*
********************************************************************************
* Description : Date display1
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp(INT8U s0, INT8U s1, INT8U s2, INT8U s3, INT8U s4) 
{
	FND_DATA[4] = s0; 
  FND_DATA[3] = s1; 
  FND_DATA[2] = s2; 
  FND_DATA[1] = s3; 
  FND_DATA[0] = s4;
}

/*
********************************************************************************
* Description : Date display2
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp2(void) 
{
	if(DVal[0] <10 && DVal[1] <10 && DVal[2] <10 && DVal[3] < 10 && DVal[4] < 10)
	  Disp(ch_num[DVal[0]], ch_num[DVal[1]], ch_num[DVal[2]], ch_num[DVal[3]], ch_num[DVal[4]]);
} 	

/*
********************************************************************************
* Description : Date display2
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp3(void) 
{
	if(DVal[0] <10 && DVal[1] <10 && DVal[2] <10 && DVal[3] < 10 && DVal[4] < 10)
	  Disp(ch_num[DVal[0]], ch_num[DVal[1]], ch_num1[DVal[2]], ch_num[DVal[3]], ch_num[DVal[4]]);
}

/*
********************************************************************************
* Description : Date display2
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp4(void) 
{
	if(DVal[0] <10 && DVal[1] <10 && DVal[2] <10 && DVal[3] < 10 && DVal[4] < 10)
	  Disp(ch_num[DVal[0]], ch_num1[DVal[1]], ch_num[DVal[2]], ch_num[DVal[3]], ch_num[DVal[4]]);
}
/*
********************************************************************************
* Description : Decimal Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void DispVal(unsigned short val) 
{
	unsigned short temp;
	
	if(val>9999) val=9999;
	DVal[4] = 0;
	DVal[3] = val/1000; temp = val%1000;
	DVal[2] = temp/100; temp = temp%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	Disp2();
}

/*
********************************************************************************
* Description : ECU Ver Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void DispEcu(unsigned short val) 
{
//	DVal[4] = 0;
//	DVal[3] = (val>>12) & 0x0F;
//	DVal[2] = (val>>8) & 0x0F;
//	DVal[1] = (val>>4) & 0x0F;
//	DVal[0] = val & 0x0F;
	unsigned short temp;
	
	if(val>9999) val=9999;
	DVal[4] = 0;
	DVal[3] = val/1000; temp = val%1000;
	DVal[2] = temp/100; temp = temp%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	Disp3();
}

/*
********************************************************************************
* Description : Dot Decimal Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp2DotVal(unsigned short val) 
{
	unsigned short temp;
	
	if(val>9999) val=9999;
	DVal[4] = 0;
	DVal[3] = val/1000; temp = val%1000;
	DVal[2] = temp/100; temp = temp%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	Disp3();
}

/*
********************************************************************************
* Description : Dot Decimal Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Disp1DotVal(unsigned short val) 
{
	unsigned short temp;
	
	if(val>9999) val=9999;
	DVal[4] = 0;
	DVal[3] = val/1000; temp = val%1000;
	DVal[2] = temp/100; temp = temp%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	Disp4();
}

/*
********************************************************************************
* Description : DPF Error Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void DispDpfErr(short val) 
{
	unsigned char temp;
	
	DVal[2] = val/100; temp = val%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	if(DVal[0]<10 && DVal[1]<10 && DVal[2]<10)
	  Disp(ch_num[DVal[0]], ch_num[DVal[1]], ch_num[DVal[2]], ch_D, ch_E);
}

/*
********************************************************************************
* Description : SCR Error Display
* Arguments   : value
* Return      : None
* Note        : 
******************************************************************************** 
*/
void DispScrErr(short val) 
{
	unsigned char temp;
	
	DVal[2] = val/100; temp = val%100;
	DVal[1] = temp/10;  temp = temp%10;
	DVal[0] = temp;
	
	if(DVal[0]<10 && DVal[1]<10 && DVal[2]<10)
	  Disp(ch_num[DVal[0]], ch_num[DVal[1]], ch_num[DVal[2]], ch_S, ch_E);
}

/* 
*********************************************************************************
*                           Keypad initialize                                   *
*********************************************************************************
*/
void KeypadInit(void) 
{
  key.newkey = NOKEY;
  key.oldkey = NOKEY;
	key.kcount = BOUNCE;
	key.kflag = 0; 
	key.kiflag = 0; 
	key.keycount = 0;
	/* clear key flags */
	memset((unsigned char *)&keyflag, 0x00, sizeof(KEYFLAG_t)); 
}


/* 
*********************************************************************************
*                           Key scan routine                                    *
*********************************************************************************
*/
unsigned short key_check(void)
{
	unsigned short key_val = NOKEY;

	if(!KEY_MODE())           key_val = MODE;
	else if(!KEY_ENTER())     key_val = ENTER;
	else if(!KEY_UP())        key_val = UP;
	else if(KEY_RESET())      key_val = RESET;
  else                      key_val = NOKEY;
	
  return(key_val);
}

/* 
*********************************************************************************
*                           buzzer process routine                              *
*********************************************************************************
*/
void BuzzerOn(INT16U t)
{
   BUZZER(ON);
   vTaskDelay(t*1000);
   BUZZER(OFF);
}


/* 
*********************************************************************************
*                           Key Check routine                                   *
*********************************************************************************
*/
void key_scan(void)
{	
	unsigned short inkey;
	
	inkey = key_check();

	if((key.newkey != inkey) && (!key.kiflag)) {
		if(key.oldkey == inkey){
			if(!key.kcount--){
				key.newkey = key.oldkey;
				key.kcount = BOUNCE;
        if(key.newkey != NOKEY)        
          key.kiflag = 1;
			}
		}
		else {
			key.oldkey = inkey;
			key.kcount = BOUNCE;
		}
	}
	else if(key.kiflag)
	{
		if((inkey == NOKEY) || (keyflag.longflag)) {
			if(!key.kcount--){
				key.kcount = BOUNCE;
				key.kflag = 1;
				key.kiflag = 0;
				key.keycount = 0;
			}
		}
		else if((key.newkey == ENTER) && (inkey == MODE)) {
			if((key.newkey != HMODE) && (!key.kcount--)) {
				key.kcount = BOUNCE;
				key.newkey = HMODE;
			}
		}
		else if((key.newkey == UP) && (inkey == MODE)) {
			if((key.newkey != CMODE) && (!key.kcount--)) {
				key.kcount = BOUNCE;
				key.newkey = CMODE;
			}
		}
		else
			key.kcount = BOUNCE;
	}
}


/* 
*********************************************************************************
*                           Key process routine                                 *
*********************************************************************************
*/
void key_process(void)
{
  key_scan();
	if(key.kflag){
		key.kflag = 0;
		switch(key.newkey) {
      case MODE:
  			keyflag.setcnt = 0;
	  		keyflag.twinkflag = 0;           
        if(!keyflag.hidden) {    
          if(!keyflag.longflag) {
            keyflag.mode++;
            if(keyflag.mode > 15) {
              keyflag.mode = 0;
              Flags.Blink = 0;
              Flags.OneEx = 0;            	
              Flags.EndFlag = 0; 
              Flags.DownloadFlag = 0;              
            }
            Flags1.DispFlag = 1;
          }  
          else {
            if(keyflag.mode == 0) {
//              Send_Regen_Start();
//       				keyflag.mode = 7;
// 		          keyflag.setcnt = 1;
              Flags1.RegenOBD = 1;
              BuzzerOn(1);
              keyflag.longflag = 0;
            }
            else keyflag.longflag = 0;
          }  
        }
        else if(keyflag.hidden==1) {
          if(!keyflag.longflag) {
            keyflag.hmode++;                                
            if(keyflag.hmode > 15) {
              keyflag.hmode = 1;
              Flags.Blink = 0;
              Flags.OneEx = 0;            	
            }
          }  
          else {
            if(keyflag.hmode == 0 ) {
              keyflag.longflag = 0;                 
            }  
          }  
        }     
        else if(keyflag.hidden==2) {
          if(!keyflag.longflag) {
            keyflag.cmode++;                                
            if(keyflag.cmode > 11) 
              keyflag.cmode = 0;
          }  
          else {
            if(keyflag.cmode == 0 ){
              keyflag.longflag = 0;                 
            }  
          }  
        }     
        break;
      case UP: 
        if(keyflag.hidden==1 || keyflag.hidden==2) {
          if(keyflag.hidden==1 && keyflag.hmode == 1 && keyflag.setcnt == 2) { 
            if(gps_on) gps_on = 0;
            else gps_on = 1;                              
          }
  			  if(keyflag.twinkflag > 0 && keyflag.twinkflag < 5) {
            if(DVal[keyflag.twinkflag-1] < 9)
              DVal[keyflag.twinkflag-1]++;
            else
              DVal[keyflag.twinkflag-1] = 0;
			 	    Disp2();
			    }             
        }
        else {
          if(!keyflag.longflag) {
              //MuteKey = 1;
          }
          else {
            if(keyflag.mode == 0) {
              // 롱키 입력시 처리 할것
              keyflag.longflag = 0;  
            }
            else if(keyflag.mode == 17) {   
              // 롱키 입력시 처리 할것
              keyflag.longflag = 0;  
            }
          }
        }
        if(keyflag.hidden==3){                                        //GSHan
     	    keyflag.setcnt = 4;
          Start_Download();      
        }      
        break;    
      case ENTER:
        if(!keyflag.hidden) {                                         //GSHan
          if(keyflag.longflag) {
             keyflag.setcnt = 0;
             keyflag.hidden = 3;
             keyflag.longflag = 0;
           }else {
     	      keyflag.setcnt++;
            Flags.Blink = 0;
            Flags.OneEx = 0;
            Flags1.DispFlag = 1;            	
            Display_Parm();
          }
        }      
        else if(keyflag.hidden==1){
     			keyflag.setcnt++;
     			Flags.Blink = 0;
          Flags.OneEx = 0;            	
          Set_Management();      
        } 
        else if(keyflag.hidden==2){
     			keyflag.setcnt++;   
        }
        else if(keyflag.hidden==3){                                   //GSHan
     	    keyflag.setcnt++;
          Start_Download();      
        }
        break;
     case RESET:
        if(keyflag.longflag) {
          // 리셋 롱키시 처리 할것
          Flags1.RegenOffOBD = 1;
          BuzzerOn(1);
          keyflag.longflag = 0;                 
        }
        break;       
     case HMODE:
       if(!keyflag.hidden) {  
			   keyflag.twinkflag = 0;  
			   keyflag.setcnt = 0;
         keyflag.hmode = 0; 
         keyflag.hidden = 1;
       }
       break;          
     case CMODE:
       if(!keyflag.hidden) {  
			   keyflag.twinkflag = 0;  
			   keyflag.setcnt = 0;
         keyflag.cmode = 0; 
         keyflag.hidden = 2;
       }
       break; 
    }
  }	
}

INT8U percent_cal(INT32U cur,INT32U max)
{
	return (cur * 100 / max);
}

/*
********************************************************************************
* Description : Hidden Mode process(Parameter)
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Start_Download(void) 
{
  if(!keyflag.setcnt)
    Disp(ch_ba, ch_G, ch_O, ch_R, ch_P);		  // Display 'Pro-'
  else if(keyflag.setcnt==1) {
    Disp(ch_ba, ch_ba, ch_qe, ch_K, ch_O);		// Display 'OK?-'  
  }
  else if(keyflag.setcnt==2) {        
    if(nDlStatus == 0) {
      if(!Flags.Blink) {
        Flags.DownloadFlag = 1;         
        Flags.Blink = 1;
      }
    }
    else if(nDlStatus == 1) {
      Disp(ch_T, ch_R, ch_A, ch_T, ch_S);		  // Display 'START'
    }
    else if(nDlStatus == 2) {
      if( (SendPacket % 10) ==0 ) {
        DispVal((unsigned short)((SendPacket*100)/TotalPacket));
      }
    }
    else if(nDlStatus == 3) {
      Disp(ch_S, ch_S, ch_E, ch_U, ch_S);		  // Display 'SUCESS'
    }
    else {
      Disp(ch_R, ch_O, ch_R, ch_R, ch_E);		  // Display 'ERROR'      
    }
  }
  else {
    nDlStatus = 0;
    Flags.Blink = 0; 
    keyflag.hidden = 0;
    Flags.StartFlag = 0;
  }
}


/*
********************************************************************************
* Description : Hidden Mode process(Parameter)
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/

struct tm	tm_rtc;
void Set_Management(void) 
{
  u16 addr;
	sysinfo_t *s = &_sysinfo;
  
	if(keyflag.hmode == 0) {
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_num[5], ch_num[5], ch_A, ch_P);  // Display 'PASS' -- Enter the password
		else if(keyflag.setcnt==1)
			DispVal(0);			// Display '0000'	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) {		
			keyflag.twinkflag = 6-keyflag.setcnt; 		//Twinkle
			Disp2();
		}
		else {
			keyflag.twinkflag = 0;			// No twinkling
			if(DVal[3]==1 && DVal[2]==1 && DVal[1]==1 && DVal[0]==1){
				Disp(ch_ba, ch_ba, ch_ba, ch_H, ch_O);	
				Flags.PasswdFlag = 1;
			}
			else{
				Disp(ch_ba, ch_ba, ch_H, ch_O, ch_N); 
				keyflag.hmode = 0;
				keyflag.setcnt = 0;
			}
		}
	}
	else if(keyflag.hmode == 1) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_ba, ch_N, ch_A, ch_C);		// Display 'CAN-'
		else if(keyflag.setcnt==1){
			DispVal(Setting.CanBPS);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.CanBPS = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, CanBPS); 
      FRAMMultiWrite((u8 *)&Setting.CanBPS, addr, sizeof(Setting.CanBPS));
      BSP_CAN_Init();
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 2) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_I, ch_H, ch_ba, ch_P);		// Display 'P-HI-'
		else if(keyflag.setcnt==1){
			DispVal(Setting.P1H_Set);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.P1H_Set = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, P1H_Set); 
      FRAMMultiWrite((u8 *)&Setting.P1H_Set, addr, sizeof(Setting.P1H_Set));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 3) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_O, ch_L, ch_ba, ch_P);		// Display 'P-Lo-'
		else if(keyflag.setcnt==1){
			DispVal(Setting.P1L_Set);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.P1L_Set = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, P1L_Set); 
      FRAMMultiWrite((u8 *)&Setting.P1L_Set, addr, sizeof(Setting.P1L_Set));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 4) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_I, ch_H, ch_ba, ch_L, ch_T);		// Display 'TL-HI'
		else if(keyflag.setcnt==1){
			DispVal(Setting.TLH_Set);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.TLH_Set = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, TLH_Set); 
      FRAMMultiWrite((u8 *)&Setting.TLH_Set, addr, sizeof(Setting.TLH_Set));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 5) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_O, ch_L, ch_ba, ch_L, ch_T);		// Display 'TL-LO'
		else if(keyflag.setcnt==1){
			DispVal(Setting.TLL_Set);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.TLL_Set = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, TLL_Set); 
      FRAMMultiWrite((u8 *)&Setting.TLL_Set, addr, sizeof(Setting.TLL_Set));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 6) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_I, ch_H, ch_ba, ch_C, ch_P);		// Display 'PC-HI'
		else if(keyflag.setcnt==1){
			DispVal(Setting.P1CalHigh);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.P1CalHigh = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, P1CalHigh); 
      FRAMMultiWrite((u8 *)&Setting.P1CalHigh, addr, sizeof(Setting.P1CalHigh));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 7) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_O, ch_L, ch_ba, ch_C, ch_P);		// Display 'PC-LO'
		else if(keyflag.setcnt==1){
			DispVal(Setting.P1CalLow);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.P1CalLow = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, P1CalLow); 
      FRAMMultiWrite((u8 *)&Setting.P1CalLow, addr, sizeof(Setting.P1CalLow));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 8) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_S, ch_F, ch_F, ch_O, ch_T);		// Display 'TOFFS'
		else if(keyflag.setcnt==1){
			DispVal(Setting.T1Offset);
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      Setting.T1Offset = DVal[3]*1000+DVal[2]*100+DVal[1]*10+DVal[0];
      addr = DRV_SETTING_PARM + offsetof(st_Setting, T1Offset); 
      FRAMMultiWrite((u8 *)&Setting.T1Offset, addr, sizeof(Setting.T1Offset));
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 9) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_Y, ch_Y, ch_Y, ch_Y);		// Display 'YYYY'
		else if(keyflag.setcnt==1){
      if(Flags.IntFlag) {
        memcpy((INT8U*)&tm_rtc, (INT8U*)&s->tm_ptr, sizeof(struct tm));
        Flags.IntFlag = 0;
      }  
			DispVal(2000+tm_rtc.tm_year);			// Display year
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
      tm_rtc.tm_year = (INT8U)(DVal[1]*10+DVal[0]);
     	SetRTCTime(&tm_rtc);
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 10) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_D, ch_D, ch_N, ch_N);		// Display 'nnDD' (Month / Date)
		else if(keyflag.setcnt==1){
      if(Flags.IntFlag) {
        memcpy((INT8U*)&tm_rtc, (INT8U*)&s->tm_ptr, sizeof(struct tm));        
        Flags.IntFlag = 0;        
      }  
			DispVal(tm_rtc.tm_mon*100+tm_rtc.tm_mday);		// Display Month / Date
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			// Twinkle & value change		
			Disp2();
		}
		else {
			tm_rtc.tm_mon = (INT8U)(DVal[3]*10+DVal[2]);	// Month
			tm_rtc.tm_mday = (INT8U)(DVal[1]*10+DVal[0]);	// Date
      SetRTCTime(&tm_rtc);
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 11) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_N, ch_N, ch_H, ch_H);		      // Display 'HHnn' (Hour - Min)
		else if(keyflag.setcnt==1){
      if(Flags.IntFlag) {
        memcpy((INT8U*)&tm_rtc, (INT8U*)&s->tm_ptr, sizeof(struct tm));        
        Flags.IntFlag = 0;
      }          
			DispVal(tm_rtc.tm_hour*100+tm_rtc.tm_min);		// Display Month / Date
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			            // Twinkle & value change		
			Disp2();
		}
		else {
			tm_rtc.tm_hour = (INT8U)(DVal[3]*10+DVal[2]);	// Hour
			tm_rtc.tm_min = (INT8U)(DVal[1]*10+DVal[0]); 	// Minute
      SetRTCTime(&tm_rtc);
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 12) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_ba, ch_C, ch_E, ch_S);		// Display 'SEC-'
		else if(keyflag.setcnt==1){
      if(Flags.IntFlag) {
        memcpy((INT8U*)&tm_rtc, (INT8U*)&s->tm_ptr, sizeof(struct tm));        
        Flags.IntFlag = 0;
      }  
			DispVal(tm_rtc.tm_sec);			    // Display Second
		}	
		else if(keyflag.setcnt>1 && keyflag.setcnt<6) { 
			keyflag.twinkflag = 6-keyflag.setcnt;			      // Twinkle & value change		
			Disp2();
		}
		else {
			tm_rtc.tm_sec = (INT8U)(DVal[1]*10+DVal[0]);	// Second
      SetRTCTime(&tm_rtc);
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;
		}
	}
	else if(keyflag.hmode == 13) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;       
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_ba, ch_R, ch_O, ch_F);		// Display 'For-'
		else if(keyflag.setcnt==1)
      Disp(ch_ba, ch_ba, ch_X, ch_H, ch_O);		// Display 'OK?-'  
		else if(keyflag.setcnt==2){
      Flags.StartFlag = 1;         
      if(!Flags.Blink){
        Disp(0x00, 0x00, 0x00, 0x00, 0x00);
        DefaultSet();         
        Flags.Blink = 1;
        SaveCount = 0;
      }
      else {      
        Disp(ch_ba, ch_ba, ch_D, ch_N, ch_E);		// Display 'End-' 
      }
		}
		else {
      Flags.Blink = 0;
      Flags.OneEx = 0;      
      Flags.StartFlag = 0;
			keyflag.setcnt = 1;
			keyflag.twinkflag = 0;      			
		}
	}
	else if(keyflag.hmode == 14) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt)
			Disp(ch_ba, ch_ba, ch_C, ch_T, ch_R);		// Display 'USER-'
		else if(keyflag.setcnt==1){
      if(GPS_Valid) {
        UpdateTimeToGPSTime(RTC_GPS_Val+TIMESHIFT_SEC);
        Disp(ch_ba, ch_ba, ch_K, ch_O, ch_ba);		// Display '-Ok--'
      }
      else Disp(ch_ba, ch_ba, ch_G, ch_N, ch_ba);		// Display '-NG-'
			keyflag.setcnt++;
		}
		else if(keyflag.setcnt==2){
			keyflag.setcnt = 0;
		}
	}		
	else if(keyflag.hmode == 15) {
		if(!Flags.PasswdFlag) 
			keyflag.hmode = 0;
		if(!keyflag.setcnt) {
			Disp(ch_ba, ch_T, ch_I, ch_H, ch_E);		//Exit
		}
		else {
			Flags.PasswdFlag = 0;
			keyflag.mode = 0;
			keyflag.setcnt = 0;
			keyflag.hidden = 0;
			Disp(0x00, 0x00, 0x00, 0x00, 0x00);		//Blank
		}
	}
} 

/*
********************************************************************************
* Description : Mode Process
* Arguments   : None
* Return      : None
* Note        : 
******************************************************************************** 
*/
void Display_Parm(void) 
{
  if(Flags1.DispFlag){
    if(keyflag.mode == 0) 
    {
      if(ReadValue.DpfValue.Error.B || ReadValue.Err.B) {
        if(ReadValue.DpfValue.Error.B) DispDpfErr(ReadValue.DpfValue.Error.B);
        else DispScrErr(ReadValue.Err.B);
      }
      else if(Flags1.TLvHAlarmFlag || Flags1.TLvLAlarmFlag)    
         Disp(ch_O, ch_L, ch_ba, ch_L, ch_U);          // UL-LO 
      else if(ReadValue.Err.UQuality)    
         Disp(ch_O, ch_C, ch_ba, ch_L, ch_U);          // UL-CO  
      else if(Disk_Full)    
         Disp(ch_L, ch_U, ch_F, ch_ba, ch_S);          // S-FUL            
      else
         DispVal(ReadValue.DpfValue.P);		  	              // P1
    }	    
    else if(keyflag.mode == 1) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_R, ch_E, ch_V);		       // Display 'VEr-'
      else	Disp2DotVal(DMS_ver);
    }
    else if(keyflag.mode == 2) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_U, ch_C, ch_E);		       // Display 'Ecu-'
      else {
        DispEcu(ReadValue.Ver);		  	
      }
    }    
    else if(keyflag.mode == 3) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_num[1], ch_ba, ch_T);		    	 // Display 'DPF-T1' (Pt200 up)
      else {
        DispVal(ReadValue.TempIn); 
      }
    } 
    else if(keyflag.mode == 4) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_num[2], ch_ba, ch_T);		    	 // Display 'DPF-T2'
      else {
        DispVal(ReadValue.DpfValue.T[1]);
      }
    } 
    else if(keyflag.mode == 5) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_num[3], ch_ba, ch_T);		    	 // Display 'DPF-T3'
      else {
        DispVal(ReadValue.DpfValue.T[2]);
      }
    } 
    else if(keyflag.mode == 6) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_num[4], ch_ba, ch_T);		    	 // Display 'DPF-T4'
      else {
        DispVal(ReadValue.DpfValue.T[3]);
      }
    }          	
    else if(keyflag.mode == 7) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_ba, ch_ba, ch_P);		      // Display 'DPF-P'
      else {
        DispVal(ReadValue.DpfValue.P);
      }
    }
    else if(keyflag.mode == 8) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_N, ch_I, ch_O, ch_N);		 			 // Display 'NOIN'
      else {
        DispVal(ReadValue.NOxIn);		  	
      }
    }
    else if(keyflag.mode == 9) {
      if(!keyflag.setcnt)
        Disp(ch_T, ch_U, ch_O, ch_O, ch_N);						// Display 'NOOUT'
      else {
        DispVal(ReadValue.NOxOut);		  	
      }
    }
//    else if(keyflag.mode == 10) {
//      if(!keyflag.setcnt)
//        Disp(ch_ba, ch_N, ch_I, ch_ba, ch_T);		 			 // Display 'In-T'    
//      else {                                           // In-T -> DPF T1
//        DispVal(ReadValue.TempIn);		  	
//      }
//    }
//    else if(keyflag.mode == 11) {
//      if(!keyflag.setcnt)
//        Disp(ch_T, ch_U, ch_O, ch_ba, ch_T);						// Display 'Out-T'
//      else {                                            // Out-T -> NonUse
//        DispVal(ReadValue.TempOut);		  	
//      }
//    }
    else if(keyflag.mode == 10) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_E, ch_T, ch_A, ch_R);		        // Display 'Rate-'
      else {
        DispVal(ReadValue.DpfValue.Rate);
      }
    }
    else if(keyflag.mode == 11) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_L, ch_U, ch_L, ch_U);		       // Display 'U-LEV'
      else {
        DispVal(ReadValue.UreaLevel);
      }
    }
    else if(keyflag.mode == 12) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_F, ch_A, ch_M);		        // Display 'MAF--'
      else {
        DispVal(ReadValue.MAF);
      }
    }
    else if(keyflag.mode == 13) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_R, ch_E, ch_P, ch_O);		        // Display 'OPER-'
      else {
        DispVal(ReadValue.Sig.Operation);
      }
    }  
    else if(keyflag.mode == 14) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_L, ch_Q, ch_ba, ch_U);		        // Display 'U-QL-'
      else {
        DispVal(ReadValue.UreaQuality);
      }
    }
    else if(keyflag.mode == 15) {
      if(!keyflag.setcnt)
        Disp(ch_ba, ch_ba, ch_P, ch_U, ch_B);		    // Display 'buP-'		
      else if(keyflag.setcnt==1) {
        Disp(ch_ba, ch_ba, ch_qe, ch_H, ch_O);		    // Display 'OK?-'
//        ExtFram.SentAddr = 0;
      }	
      else if(keyflag.setcnt==2) {
        Flags.StartFlag = 1;
        if(Disk_Dtect() && !Flags.OneEx) {              
          Disp(0x00, 0x00, 0x00, 0x00, 0x00);    		                        
          BackUpFlag = 1;
//          SPI_FLASH_BackUP();
          BackUpFlag = 0;        
          Flags.OneEx = 1;
        }	
        Disp(ch_ba, ch_ba, ch_D, ch_N, ch_E);	    // Display 'End-'         
        Flags.SaveOkFlag = 0;
      } 
      else {
        keyflag.setcnt = 1;
        Flags.Blink = 0;
        Flags.OneEx = 0;            	
        Flags.StartFlag = 0;
        Flags.EndFlag = 0;             
      }
    }
  Flags1.DispFlag = 0;
  }
}
    
/* 
*********************************************************************************
*                           Key process routine                                 *
*********************************************************************************
*/
void KeySemaphore_Init(void)
{
	BSP_KeySem = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(BSP_KeySem);
}


/* 
*********************************************************************************
*                           Key process routine                                 *
*********************************************************************************
*/
void Task_KEY(void *p_arg)
{

	sysinfo_t *s = (sysinfo_t *)p_arg 	;
  KeySemaphore_Init();
	
  for(;;)
	{
		s->twatchKeyFlag = 1;
    key_process();
    if(!keyflag.hidden)
      Display_Parm();    
    else if(keyflag.hidden==1)
      Set_Management();
    else if(keyflag.hidden==2){
    }
    else if(keyflag.hidden==3){                              //GSHan
      Start_Download();
    }
		s->twatchKeyFlag = 0;
    vTaskDelay(1);
	}
}


/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/