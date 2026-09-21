#include <stdio.h>
#include <wctype.h>
#include <limits.h>
#include <fenv.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <ctype.h>
#include "murks_func056.h"
#include <stdlib.h>
#include <wchar.h>
#include <stdint.h>
#include <locale.h>
#include <assert.h>
#include <stdbool.h>
#include <float.h>
#include <setjmp.h>
#include <iso646.h>
#include <stddef.h>
void murks_func056(void){
UDINT lI=(UDINT)floorf(0.0f); 
UDINT Il=0u;
UDINT Ol=0u;
UINT lo=0;
for/**/( lo=0; lo<16; lo++){
Il=(UDINT)lo<<1u;
if/**/( (Il&0x10u)!=0u){
lI=lI|(1u<<(lo%8u));
Ol=Ol|0x01u;
}else{
lI=lI&~(UDINT)(1u<<(lo%8u));
Ol=Ol&~0x01u;
}
if/**/( (lI&0xFFu)==0xA5u){
Il=Il^0xFFFFu;
}
}
switch/**/( (lI|Il)%7u){
case 0u:{Ol=0u; break;}
case 1u:{Ol=lI; break;}
case 2u:{Ol=Il; break;}
case 3u:{lI=Ol; break;}
case 4u:{Il=Ol; break;}
case 5u:{lI^=Il; break;}
case 6u:{Il^=lI; break;}
default:{break;}
}
if/**/( (Ol!=0u)&&((lI&Il)==0u)){
lI=Ol|Il;
}
}
