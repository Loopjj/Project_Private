/*
Copyright (c) 2005, David M Howard (daveh at dmh2000.com)
All rights reserved.

This product is licensed for use and distribution under the BSD Open Source License.
see the file COPYING for more details.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

*/
#define GPS_GLOBALS
#include "includes.h"


//INT16U checktime = 0;
static nmeap_context_t nmea;	    /* parser context */

/* this only works if you are sure you have an upper case hex digit */
//#define HEXTOBIN(ch) ((ch <= '9') ? ch - '0' : ch - ('A' - 10))

/* forward references */
int nmeap_init(nmeap_context_t *context,void *user_data);
int nmeap_addParser(nmeap_context_t        *context,
                    const char             *sentence_name,
                    nmeap_sentence_parser_t sentence_parser,
                    nmeap_callout_t         sentence_callout,
                    void                   *sentence_data
                   );
int nmeap_tokenize(nmeap_context_t *context);
int nmeap_process(nmeap_context_t *context);
int nmeap_parse(nmeap_context_t *context,char ch);
int nmeap_parseBuffer(nmeap_context_t *context,const char *buffer,int *length);


/*
********************************************************************************
* Description : nmeap_latitude
* Arguments   : const char *plat,const char *phem
* Return      : 
* Note        : get a latitude out of a pair of nmea tokens 
******************************************************************************** 
*/
double nmeap_latitude(const char *plat,const char *phem)
{
    double lat;
    int    deg;
    double min;
    int    ns;
    
    assert(plat != 0);
    assert(phem != 0);
    
    if (*plat == 0) {
        return 0.0;
    }
    if (*phem == 0) {
        return 0.0;
    }
    
    /* north lat is +, south lat is - */
    if (*phem == 'N') {
        ns = 1;
    }
    else {
        ns = -1;
    }
    
    /* latitude is degrees, minutes, fractional minutes */
    /* no validation is performed on the token. it better be good.*/
    /* if it comes back 0.0 then probably the token was bad */
    lat = atof(plat);
    
    /* extract the degree part */
    deg = (int)(lat / 100.0);
    
    /* mask out the degrees */
    min = lat - (deg * 100.0);
    
    /* compute the actual latitude in degrees.decimal-degrees */
    lat = (deg + (min / 60.0)) * ns;
    
    return lat;
}


/*
********************************************************************************
* Description : nmeap_longitude
* Arguments   : const char *plon,const char *phem
* Return      : 
* Note        :  get a longitude out of a pair of nmea tokens
******************************************************************************** 
*/
double nmeap_longitude(const char *plon,const char *phem)
{
    double lon;
    int    deg;
    double min;
    int    ew;
    
    assert(plon != 0);
    assert(phem != 0);
    
    if (*plon == 0) {
        return 0.0;
    }
    if (*phem == 0) {
        return 0.0;
    }
    
    /* west long is negative, east long is positive */
    if (*phem == 'E') {
        ew = 1;
    }
    else {
        ew = -1;
    }
    
    /* longitude is degrees, minutes, fractional minutes */
    /* no validation is performed on the token. it better be good.*/
    /* if it comes back 0.0 then probably the token was bad */
    lon = atof(plon);
    
    /* extract the degree part */
    deg = (int)(lon / 100.0);
    
    /* mask out the degrees */
    min = lon - (deg * 100.0);
    
    /* compute the actual lonitude in degrees.decimal-degrees */
    lon = (deg + (min / 60.0)) * ew;

    
    return lon;
}


/*
********************************************************************************
* Description : nmeap_altitude
* Arguments   : const char *palt,const char *punits
* Return      : 
* Note        :  get an altitude longitude out of a pair of nmea tokens
*                ALTITUDE is returned in METERS 
******************************************************************************** 
*/
double nmeap_altitude(const char *palt,const char *punits)
{
    double alt;
    
    if (*palt == 0) {
        return 0.0;
    }
    
    /* convert with no error checking */
    alt = atof(palt);
    
    if (*punits == 'M') {
        /* already in meters */ 
    }
    else if (*punits == 'F') {
        /* convert to feet */
        alt = alt * 3.2808399;
    }
    
    return alt;
}


/*
********************************************************************************
* Description : nmeap_init
* Arguments   : nmeap_context_t *context,void *user_data
* Return      : 
* Note        : initialize an NMEA parser
******************************************************************************** 
*/
int nmeap_init(nmeap_context_t *context,void *user_data)
{
    assert(context != 0);

    memset(context,0,sizeof(*context));

    context->user_data = user_data;

    return 0;    
}


/*
********************************************************************************
* Description : nmeap_addParser
* Arguments   : ...
* Return      : 
* Note        : register an NMEA sentence parser
******************************************************************************** 
*/
int nmeap_addParser(nmeap_context_t        *context,
					          const char             *sentence_name,
					          nmeap_sentence_parser_t sentence_parser,
					          nmeap_callout_t         sentence_callout,
                    void                   *sentence_data
					         )
{
    nmeap_sentence_t *s = 0;
    
    /* runtime error */
    assert(context != 0);
    
    /* sentence capacity overflow */
    if (context->sentence_count >= NMEAP_MAX_SENTENCES) {
        return -1;
    }
    
    /* point at next empty sentence buffer */
    s = &context->sentence[context->sentence_count];
    
    /* advance sentence data count */
    context->sentence_count++;
    
    /* clear the sentence data */
    memset(s,0,sizeof(*s));
    
    /* name */
    strncpy(s->name,sentence_name,NMEAP_MAX_SENTENCE_NAME_LENGTH);
    
    /* parser */
    s->parser = sentence_parser;
    
    /* callout */
    s->callout = sentence_callout;
    
    /* data */
    s->data    = sentence_data;

    return 0;
}


/*
********************************************************************************
* Description : nmeap_tokenize
* Arguments   : nmeap_context_t *context
* Return      : 
* Note        : tokenize a buffer
******************************************************************************** 
*/
int nmeap_tokenize(nmeap_context_t *context)
{
    char *s;
    int   tokens;
    int   state;
    
    /* first token is header. assume it is there */
    tokens = 0;
    s = context->input;
    context->token[tokens] = s;
    
    /* get rest of tokens */
    tokens = 1;
    state = 0;
    while((*s != 0)&&(tokens < NMEAP_MAX_TOKENS)) {
        switch(state) {
        case 0:
            /* looking for end of a token */
            if (*s == ',') {
                /* delimit at the comma */
                *s    = 0;
                /* new token */
                state = 1;
            }
            break;
        case 1:
            /* start of next token, might be another comma */
            context->token[tokens++] = s;
            if (*s == ',') {
                /* delimit at the comma */
                *s    = 0;
            }
            else {   
                /* not a comma */
                state = 0;
            }
            break;
        default:
            state = 0;
            break;
        }
        
        // next character
        s++;
    }
    return tokens;
}


/*
********************************************************************************
* Description : nmeap_process
* Arguments   : nmeap_context_t *context
* Return      : 
* Note        : process a sentence
******************************************************************************** 
*/
int nmeap_process(nmeap_context_t *context)
{
    int id;
    int i;
    nmeap_sentence_t *s;
	
    /* copy the input to a debug buffer */
    /* remove debug_input when everything is working. */
    strncpy(context->debug_input,context->input,sizeof(context->debug_input));
    
    /* tokenize the input */
    context->tokens = nmeap_tokenize(context);
    
    /* try to find a matching sentence parser */
    /* this search is O(n). it has a lot of potential for optimization, */
    /* at the expense of complexity, if you have a lot of sentences */
    /* binary search instead of linear (have to keep sentences in sorted order) O(NlogN) */
    /* OR, when sentences are added, create a TRIE structure to find the names with */
    /* a constant time search O(5) */
    for(i=0;i<context->sentence_count;i++) {
        s = &context->sentence[i];
        assert(s != 0);
        if (strncmp(context->input_name,s->name,5) == 0) {
            /* found a match, call its parser */
            id = (*context->sentence[i].parser)(context,s);
            if (id > 0) {
                break;
            }
        }
    }
        
    return id;
}

/*
********************************************************************************
* Description : nmeap_parse
* Arguments   : nmeap_context_t *context,char ch
* Return      : 
* Note        : 
                 +-5-+              +---+
                 v   |              v   |
 +------+       +------+           +------+         +------+         +------+
 |   0  |--$--> |1-hdr |--alnum--> |2-data|----\r-->| 6-LF |---\n--->| done |--> 0
 +------+       +------+           +------+         +------+         +------+
                                    |                 ^
                                    *                 +--------\r-------+
                                    V                                   |
                                 +------+           +------+         +------+
                                 |3-cks |--xdigit-->|4-cks |-xdigit->| 5-CR |
                                 +------+           +------+         +------+
                                 
return to start conditions:
1. buffer overflow
2. invalid character for state

checksum calculation
two hex digits represent the XOR of all characters between, but not
including, the "$" and "*".  A checksum is required on some
sentences.
******************************************************************************** 
*/
int nmeap_parse(nmeap_context_t *context,char ch)
{
  int status = 0;
  
  /* check for input buffer overrun first to avoid duplicating code in the
  individual states
  */
  if (context->input_count >= (sizeof(context->input)-1)) {
    /* input buffer overrun, restart state machine */
    context->input_state = 0;
    /* reset input count */
    context->input_count = 0;
  }
  
  /* store the byte */
  context->input[context->input_count] = ch;
  
  /* next buffer position */
  context->input_count++;
  
  /* run it through the lexical scanner */
  switch(context->input_state) {
  /* LOOKING FOR $ */
  case 0:
    if (ch == '$') {
        /*look for id */
        context->input_state = 1;
        context->ccks        = 0;
        context->icks        = 0;
    }
    else {
        /* header error, start over */
        context->err_hdr++;
        context->input_state = 0;
        context->input_count = 0;
    }
    break;
  /* LOOKING FOR 5 CHARACTER SENTENCE ID */
  case 1:
    /* allow numbers even though it isn't usually done */
    /* a proprietary id might have a numeral */
    if (isalnum(ch)) {
        /* store name separately */
        context->input_name[context->input_count - 2] = ch;
        /* checksum */
        context->ccks ^= ch;
        /* end of header? */
        if (context->input_count >= 6) {
            /* yes, get body */
            context->input_state = 2;
        }
    }
    else {
        /* bad character, start over */
        context->err_id++;
        context->input_state = 0;
        context->input_count = 0;
    }
    break;
  /* LOOKING FOR CR OR CHECKSUM INDICATOR */
  case 2:
    if (ch == '*') {
        /* this sentence has a checksum */
        context->input_state = 3;
    }
    else if (ch == '\r') {
        /* carriage return, no checksum, force a match */
      context->icks = 0;
      context->ccks = 0;
      context->input_state = 6;
    }
    else {
        /* continue accumulating data */
        /* checksum */
        context->ccks ^= ch;
    }
    break;
  /* LOOKING FOR FIRST CHECKSUM CHARACTER */
  case 3:
    /* must be upper case hex digit */
    if (isxdigit(ch) && (ch <= 'F')) {
      /* got first checksum byte */
      context->input_state = 4;
      context->icks = HEXTOBIN(ch) << 4;
    }
    else {
      /* input error, restart */
      context->err_cks++;
      context->input_state = 0;
      context->input_count = 0;
    }
    break;
    /* LOOKING FOR SECOND CHECKSUM CHARACTER */
  case 4:
    /* must be upper case hex digit */
    if (isxdigit(ch) && (ch <= 'F')) {
        /* got second checksum byte */
        context->input_state = 5;
        context->icks += HEXTOBIN(ch);
    }
    else {
        /* input error, restart */
        context->err_cks++;
        context->input_state = 0;
        context->input_count = 0;
    }
    break;
  /* LOOKING FOR CR */
  case 5:
    if (ch == '\r') {
        /* carriage return */
        context->input_state = 6;
    }
    else {
        /* input error, restart */
        context->err_crl++;
        context->input_state = 0;
        context->input_count = 0;
    }
    break;
  /* LOOKING FOR LINE FEED */
  case 6:
    if (ch == '\n') {
      /* linefeed, line complete */
      
      /* delimit buffer */
      context->input[context->input_count] = 0;
          
      /* if the checksums match, process the sentence */
      if (context->ccks == context->icks) {
        /* process */
        status = nmeap_process(context);
        //gps_status = status;
        /* count good messages */
        context->msgs++;
      }
      else {
        /* count checksum errors */
        context->err_cks++;
      }
          
      /* restart next time */
      context->input_state = 0;
      context->input_count = 0;
    }
    else {
        /* input error, restart */
        context->err_crl++;
        context->input_state = 0;
        context->input_count = 0;
    }
    break;
  default:
    context->err_unk++;
    context->input_state = 0;
    break;
  }
            
	return status;
}


/*
********************************************************************************
* Description : nmeap_gpgga
* Arguments   : nmeap_context_t *context,const char *buffer,int *length
* Return      : 
* Note        : parse a buffer of nmea data
******************************************************************************** 
*/
int nmeap_parseBuffer(nmeap_context_t *context,const char *buffer,int *length)
{
  int  i;
  int  status;
  int  rem;
  int  tlen;
  
  tlen   = *length;
  rem    = *length;
  status = 0;
  /* for each byte in the buffer */
  for(i=0;i<tlen;i++) {
    /* decrement remaining byte count */
    rem--;
    /* parse the byte */
    status = nmeap_parse(context,buffer[i]);
    if (status != 0) {
      /* message found or error */
      break;
    }
  }

  /* return remaining byte count */
  *length = rem;

  return status;
}


/*
********************************************************************************
* Description : nmeap_gpgga
* Arguments   : nmeap_context_t *context,nmeap_sentence_t *sentence
* Return      : 
* Note        : standard GPGGA sentence parser 
******************************************************************************** 
*/
int nmeap_gpgga(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
  int i;

  
  /* get pointer to sentence data */
  nmeap_gga_t *gga = (nmeap_gga_t *)sentence->data;
  
  /* if there is a data element, extract data from the tokens */
	if (gga != 0) {
		gga->latitude  = nmeap_latitude(context->token[2],context->token[3]);
		gga->longitude = nmeap_longitude(context->token[4],context->token[5]);
		gga->altitude  = nmeap_altitude(context->token[9],context->token[10]);
		gga->time       = atoi(context->token[1]);
		gga->satellites = atoi(context->token[7]);
		gga->quality    = atoi(context->token[6]);
		gga->hdop       = atof(context->token[8]);
		gga->geoid      = nmeap_altitude(context->token[11],context->token[12]);
	}

  if(gps_on) { 
    /* print raw input string */
    printk("%s",context->debug_input);
    
    /* print some validation data */
    printk("%s==%s %02x==%02x\n",context->input_name,sentence->name,context->icks,context->ccks);
    
    /* print the tokens */
    for(i=0;i<context->tokens;i++) {
        printk("%d:%s\n",i,context->token[i]);
    }
  } 

  /* if the sentence has a callout, call it */
  if (sentence->callout != 0) {
      (*sentence->callout)(context,gga,context->user_data);
  }
  
  return NMEAP_GPGGA;
}


/*
********************************************************************************
* Description : nmeap_gprmc
* Arguments   : nmeap_context_t *context,nmeap_sentence_t *sentence
* Return      : 
* Note        : standard GPRMCntence parser 
******************************************************************************** 
*/
int nmeap_gprmc(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
  int i;


  /* get pointer to sentence data */
  nmeap_rmc_t *rmc = (nmeap_rmc_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (rmc != 0) {
		/* extract data from the tokens */
		rmc->time       = atoi(context->token[1]);
		rmc->warn       = *context->token[2];
		rmc->latitude  = nmeap_latitude(context->token[3],context->token[4]);
		rmc->longitude = nmeap_longitude(context->token[5],context->token[6]);
		rmc->speed      = atof(context->token[7]);
		rmc->course     = atof(context->token[8]);
		rmc->date       = atoi(context->token[9]);
		rmc->magvar     = atof(context->token[10]);
    GPS_SigWait_Cnt = 0;
	}

  if(gps_on) {   
    /* print raw input string */
    printk("%s",context->debug_input);
    
    /* print some validation data */
    printk("%s==%s %02x==%02x\n",context->input_name,sentence->name,context->icks,context->ccks);
    
    /* print the tokens */
    for(i=0;i<context->tokens;i++) {
        printk("%d:%s\n",i,context->token[i]);
    }
  }   

  /* if the sentence has a callout, call it */
  if (sentence->callout != 0) {
      (*sentence->callout)(context,rmc,context->user_data);
  }
  
  return NMEAP_GPRMC;
}

/*
********************************************************************************
* Description : nmeap_gprmc
* Arguments   : nmeap_context_t *context,nmeap_sentence_t *sentence
* Return      : 
* Note        : standard GPRMCntence parser 
******************************************************************************** 
*/
int nmeap_gpgsa(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
  int i;


  /* get pointer to sentence data */
  nmeap_gsa_t *gsa = (nmeap_gsa_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (gsa != 0) {
		/* extract data from the tokens */
		gsa->pdop       = atof(context->token[15]);
		gsa->hdop       = atof(context->token[16]);
		gsa->vdop       = atof(context->token[17]);
	}

  if(gps_on) {   
    /* print raw input string */
    printk("%s",context->debug_input);
    
    /* print some validation data */
    printk("%s==%s %02x==%02x\n",context->input_name,sentence->name,context->icks,context->ccks);
    
    /* print the tokens */
    for(i=0;i<context->tokens;i++) {
        printk("%d:%s\n",i,context->token[i]);
    }
  }   

  /* if the sentence has a callout, call it */
  if (sentence->callout != 0) {
      (*sentence->callout)(context,gsa,context->user_data);
  }
  
  return NMEAP_GPGSA;
}
                    

/*
********************************************************************************
* Description : print_gga
* Arguments   : nmeap_rmc_t *rmc
* Return      : 
* Note        : do something with the GGA data
******************************************************************************** 
*/
static void print_gga(nmeap_gga_t *gga)
{
    printk("found GPGGA message %.6f %.6f %.0f %lu %d %d %f %f\n",
            gga->latitude  ,
            gga->longitude, 
            gga->altitude , 
            gga->time     , 
            gga->satellites,
            gga->quality   ,
            gga->hdop      ,
            gga->geoid     
            );
}

/** 
********************************************************************************
* Description : gpgga_callout
* Arguments   : nmeap_context_t *context,void *data,void *user_data
* Return      : 
* Note        : called when a gpgga message is received and parsed
******************************************************************************** 
*/
static void gpgga_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_gga_t *gga = (nmeap_gga_t *)data;

    if(gps_on) print_gga(gga);
}


/*
********************************************************************************
* Description : print_rmc
* Arguments   : nmeap_rmc_t *rmc
* Return      : 
* Note        : do something with the RMC data
******************************************************************************** 
*/
static void print_rmc(nmeap_rmc_t *rmc)
{
    printk("found GPRMC Message %lu %c %.6f %.6f %f %f %lu %f\n",
            rmc->time,
            rmc->warn,
            rmc->latitude,
            rmc->longitude,
            rmc->speed,
            rmc->course,
            rmc->date,
            rmc->magvar
            );
}

/*
********************************************************************************
* Description : gprmc_callout
* Arguments   : nmeap_context_t *context,void *data,void *user_data
* Return      : 
* Note        : called when a gprmc message is received and parsed 
******************************************************************************** 
*/
static void gprmc_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_rmc_t *rmc = (nmeap_rmc_t *)data;

    if(gps_on) print_rmc(rmc);
}

/*
********************************************************************************
* Description : print_gsa
* Arguments   : nmeap_gsa_t *gsa
* Return      : 
* Note        : do something with the GGA data
******************************************************************************** 
*/
static void print_gsa(nmeap_gsa_t *gsa)
{
    printk("found GPGSA message %f %f %f\n",
            gsa->pdop      ,
            gsa->hdop      ,
            gsa->vdop     
            );
}

/** 
********************************************************************************
* Description : gpgsa_callout
* Arguments   : nmeap_context_t *context,void *data,void *user_data
* Return      : 
* Note        : called when a gpgga message is received and parsed
******************************************************************************** 
*/
static void gpgsa_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_gsa_t *gsa = (nmeap_gsa_t *)data;

    if(gps_on) print_gsa(gsa);
}

/*
********************************************************************************
* Description : GPS_parse_proc
* Arguments   : none
* Return      : 
* Note        : GPS Data parsing process
******************************************************************************** 
*/
void GPS_parse_proc(void)	
{
	INT16U tmptail;  
	char data=0;
  int  status = 0; 
     
	while (GPS_RxHead != GPS_RxTail ) { 
    tmptail = (GPS_RxTail + 1) & GPS_RX_BUFFER_MASK; /* Calculate buffer index */
    
    GPS_RxTail = tmptail;                /* Store new index */
    
    data = GPS_RxBuf[tmptail];           /* Return data */   
    
    status = nmeap_parse(&nmea, data);
    GPS_Reset_Cnt = 0;
    if(gps_on) {
      switch(status) {
      case NMEAP_GPGGA:
          /* GPGGA MESSAGE */
          printk("-------------switch\n");
          print_gga(&gga);
          printk("-------------\n");
          break;
      case NMEAP_GPRMC:
          /* GPRMC MESSAGE */
          printk("-------------switch\n");
          print_rmc(&rmc);
          printk("-------------\n");
          break;
      case NMEAP_GPGSA:
          /* GPGSA MESSAGE */
          printk("-------------switch\n");
          print_gsa(&gsa);
          printk("-------------\n");
          break;
      default:
          break;
      }      
    }
  }
}

/*
********************************************************************************
* Description : GPS_SecTimerProcess
* Arguments   : none
* Return      : 
* Note        : GPS 1Sec Timer Process
******************************************************************************** 
*/
#define GPS_RESET_ENABLE
#define GPS_RESET_INTERVAL 30
#define GPS_RESET_TIME 2

void GPS_ResetProcess(void) 
{
#ifdef GPS_RESET_ENABLE  
  if(Power_Off_Flag) return;
  GPS_Reset_Cnt++;    
  if(GPS_Reset_Cnt == GPS_RESET_INTERVAL) {
    GPS_Reset_Flag = 1;
    GPS_POWER_OFF();  
  }    
  else if(GPS_Reset_Cnt == (GPS_RESET_INTERVAL+GPS_RESET_TIME)) {
    GPS_Reset_Cnt = 0;
    GPS_POWER_ON();
    GPS_Reset_Flag = 0;
  }
#endif  
}

/*
********************************************************************************
* Description : dbg_gps_view
* Arguments   : None
* Return      : 
* Note        :
******************************************************************************** 
*/
INT16U dbg_gps_view(void)
{
	if ( gps_on )
		gps_on = 0;
	else
		gps_on = 1;

	return NO_DISPLAY;
}

                    
/*
********************************************************************************
* Description : Task_GPS
* Arguments   : none
* Return      : 
* Note        : task for gps
******************************************************************************** 
*/
                    
/* Task_GPS function */
void Task_GPS(void const * argument)
{
  int  status;
  int  task_count = 0;
  
	sysinfo_t *s = (sysinfo_t *)argument;
   
  status = nmeap_addParser(&nmea,"GPGGA",nmeap_gpgga,gpgga_callout,&gga);
  if (status != 0) {
      printd("nmeap_add %d\n",status);
  }

  status = nmeap_addParser(&nmea,"GPRMC",nmeap_gprmc,gprmc_callout,&rmc);
  if (status != 0) {
      printd("nmeap_add %d\n",status);
  }

  status = nmeap_addParser(&nmea,"GPGSA",nmeap_gpgsa,gpgsa_callout,&gsa);
  if (status != 0) {
      printd("nmeap_add %d\n",status);
  }
  GPS_Valid_Char = 'N';
  GPS_POWER_ON();     // GPS Power On
  
  /* USER CODE BEGIN Task_GPS */
  /* Infinite loop */
  for(;;)
	{
		s->twatchGpsFlag = 1;    
    GPS_parse_proc();
    if((task_count%500) == 0) {
      //if(rmc.warn =='A' && gga.satellites > 3 && gga.hdop < 5.0 && GPS_SigWait_Cnt < 3)  {
      if(rmc.warn =='A'&& GPS_SigWait_Cnt < 3) {  
        GPS_Valid = 1;
        GPS_Valid_Char = 'G';  
        GPS_Not_Valid_Cnt = 0;
        GPS_Fault = 0;   
      }
      else {
        GPS_Valid = 0;
        if(GPS_SigWait_Cnt < 3)
          GPS_Valid_Char = rmc.warn;
        else
          GPS_Valid_Char = 'N';
//        if(GPS_Not_Valid_Cnt > 5) 
        GPS_Fault = 1;
      }
    }
    task_count++;
    if(task_count > 999) {
      task_count = 0;
      if(!GPS_Reset_Flag) {
        if(!GPS_Valid) GPS_Not_Valid_Cnt++;
        if(GPS_SigWait_Cnt < 0xFF) GPS_SigWait_Cnt++;
      }
      GPS_ResetProcess();
    }
		s->twatchGpsFlag = 0;  
    osDelay(1);
	}
  /* USER CODE END Task_GPS */  
}


/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/