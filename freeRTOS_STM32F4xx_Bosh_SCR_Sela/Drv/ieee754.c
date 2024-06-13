/*
********************************************************************************	
*
*              (c) Copyright 2006, loopsystem
*                      All rights reserved
*
* Filename    : ieee754.c
* Description : IEEE floating-point standard
********************************************************************************
*/
#include "ieee754.h"

double arrayToFloat(char* data)
{
   int s, e;
   unsigned long src;
   double f;
   double value;
 
   src = ((unsigned long)(data[3] & 0x000000FF) << 24) + 
         ((unsigned long)(data[2] & 0x000000FF) << 16) +
         ((unsigned long)(data[1] & 0x000000FF) << 8) +
         ((unsigned long)(data[0] & 0x000000FF));
 
   s = (src & 0x80000000UL) >> 31;
   e = (src & 0x7F800000UL) >> 23;
   f = (src & 0x007FFFFFUL);
 
   if (e == 255 && f != 0) {
     /* NaN - Not a number */
     value = DBL_MAX;
   }
   else if (e == 255 && f == 0 && s == 1) {
     /* Negative infinity */
     value = -DBL_MAX;
   }
   else if (e == 255 && f == 0 && s == 0) {
     /* Positive infinity */
     value = DBL_MAX;
   }
   else if (e > 0 && e < 255) {
     /* Normal number */
     f += 0x00800000UL;
     if (s) f = -f;
     value = ldexp(f, e - 127 - 23);
     }
   else if (e == 0 && f != 0) {
     /* Denormal number */
     if (s) f = -f;
     value = ldexp(f, -126 - 23);
   }
   else if (e == 0 && f == 0 && s == 1) {
     /* Negative zero */
     value = 0;
   }
   else if (e == 0 && f == 0 && s == 0) {
     /* Positive zero */
     value = 0;
   }
   else {
     /* Never happens */
   }
 
   return value;
 }
