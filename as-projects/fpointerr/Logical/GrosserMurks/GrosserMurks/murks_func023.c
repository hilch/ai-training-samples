#include <wctype.h>
#include <iso646.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>
#include <stdio.h>
#include <float.h>
#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <fenv.h>
#include "murks_func023.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <inttypes.h>
void murks_func023(void){
UINT lI=(UINT)tanf(0.0f); 
UINT Il=0u;
UINT Ol=0u;
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<256; oI+=16){
Il=(oI&0xE0u)>>5u;
Ol=(oI&0x1Cu)>>2u;
lo=(oI&0x03u);
if/**/( Il>4u){
Il=7u-Il;
}
if/**/( Ol>3u){
Ol=Ol^0x07u;
}
lI=(Il<<5u)|(Ol<<2u)|lo;
}
switch/**/( lI&0x03u){
case 0u:{lo=0u; break;}
case 1u:{lo=1u; break;}
case 2u:{lo=2u; break;}
case 3u:{lo=3u; break;}
default:{break;}
}
if/**/( (Il+Ol+lo)<20u){
lI=(lI+1u)&0xFFu;
}
}
