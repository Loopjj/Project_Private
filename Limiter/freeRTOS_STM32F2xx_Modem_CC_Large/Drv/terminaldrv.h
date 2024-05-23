/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : terminaldrv.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 07/05/2014
*
* Description        : terminal driver configuration
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef TERMINALDRV_H
#define TERMINALDRV_H



/*
*********************************************************************************************************
*                                             LOCALE DEFINES
*********************************************************************************************************
*/

/*------- Text modes --------*/
#define MODE_NONE			'0'
#define MODE_BOLD			'1'
#define MODE_DIM			'2'
#define MODE_UNDERLINE		'4'
#define MODE_BLINK			'5'
#define MODE_REVERSED		'7'
#define MODE_CONCEALED		'8'


/*------ Text colours -------*/
#define COL_FOREGROUND		'3'
#define COL_BACKGROUND		'4'

#define COL_BLACK			'0'
#define COL_RED				'1'
#define COL_GREEN			'2'
#define COL_YELLOW			'3'
#define COL_BLUE			'4'
#define COL_MAGENTA			'5'
#define COL_CYAN			'6'
#define COL_WHITE			'7'


/*-- Cursor move direction --*/
#define MOVE_UP				'A'
#define MOVE_DOWN			'B'
#define MOVE_RIGHT			'C'
#define MOVE_LEFT			'D'

#define __INLINE inline 


/*
*********************************************************************************************************
*                                         INLINE FUNCTION PROTOTYPES
*********************************************************************************************************
*/


#ifndef ANSI_MODULE
#define ANSI_MODULE
#endif


/*-- Send escape sequence start --*/
#define SENDESC		\
	Term_Send( 27 );	\
	Term_Send( '[' );



/*-- Transmit one byte --*/
__INLINE void Term_Send( unsigned char data )
{
	put_ch_console(data);							
}


/*-- Set text mode --*/
__INLINE void Term_Set_Display_Attribute_Mode(unsigned char mode)
{
#ifdef ANSI_MODULE
	SENDESC                     

	Term_Send( mode );
	Term_Send( 'm' );
#endif
}


/*-- Convert byte to 3 ASCII digits and send --*/
__INLINE void Term_Send_Value_as_Digits(unsigned char value)
{
    unsigned char digit;
    
    digit = '0';
    while( value >= 100 )                // Still larger than 100 ?
    {
        digit++;                         // Increment first digit
        value -= 100;
    }
    
    Term_Send( digit );                  // Send first digit
    
    digit = '0';
    while( value >= 10 )                 // Still larger than 10 ?
    {
        digit++;                         // Increment second digit
        value -= 10;
    }
    
    Term_Send( digit );                  // Send second digit
    
    Term_Send( '0' + value );            // Send third digit
}


/*-- Set cursor position / Top-left is (1,1) --*/
__INLINE void Term_Set_Cursor_Position(unsigned char row, unsigned char column)
{
#ifdef ANSI_MODULE
	SENDESC									

	Term_Send_Value_as_Digits(row);		// Convert row byte
	Term_Send( ';' );
	Term_Send_Value_as_Digits(column);	// Convert column byte
	Term_Send( 'H' );
#endif
}


/*-- Send 'clear line' sequence --*/
__INLINE void Term_Erase_Line(void)
{
#ifdef ANSI_MODULE
	SENDESC                             

	Term_Send( '2' );
	Term_Send( 'K' );
#endif
}


/*-- Set text colour --*/
__INLINE void Term_Set_Display_Colour(unsigned char fg_bg, unsigned char colour)
{
#ifdef ANSI_MODULE
	SENDESC									

	Term_Send( fg_bg );					// Select foreground/background
	Term_Send( colour );
	Term_Send( 'm' );
#endif

}


/*-- Move cursor --*/
__INLINE void Term_Move_Cursor(unsigned char distance, unsigned char direction)
{
#ifdef ANSI_MODULE
	SENDESC                             	

	Term_Send_Value_as_Digits( distance );	// Convert distance byte

	Term_Send( direction );
#endif
}


/*-- Save cursor position --*/
__INLINE void Term_Save_Cursor_Position(void)
{
#ifdef ANSI_MODULE
	SENDESC                             	

	Term_Send( 's' );
#endif
}


/*-- Restore cursor position --*/
__INLINE void Term_Restore_Cursor_Position(void)
{
#ifdef ANSI_MODULE
	SENDESC									

	Term_Send( 'u' );
#endif
}




#endif 
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
