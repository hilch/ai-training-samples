#include <errno.h>
#include <stdlib.h>
#include "murks_func040.h"
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <iso646.h>
#include <ctype.h>
#include <inttypes.h>
#include <wchar.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <fenv.h>
void murks_func040(void){
UDINT lI=(UDINT)sinf(0.0f); 
UDINT Il=0u;
UINT Ol=0xA5u;
UINT lo=0;
USINT oI[4]={0x48u, 0x65u, 0x6Cu, 0x6Cu};
for/**/( lo=0; lo<4; lo++){
USINT Io=(USINT)((UINT)oI[lo]^Ol);
lI=(lI<<8u)|(UDINT)Io;
Ol=(UINT)(((Ol<<1u)|(Ol>>7u))&0xFFu);
}
Ol=0xA5u;
for/**/( lo=0; lo<4; lo++){
USINT lO=(USINT)((lI>>(24u-lo*8u))&0xFFu);
USINT ll=(USINT)((UINT)lO^Ol);
Il=(Il<<8u)|(UDINT)ll;
Ol=(UINT)(((Ol<<1u)|(Ol>>7u))&0xFFu);
}
switch/**/( (lI^Il)%3u){
case 0u:{lI^=0xFFFFFFFFu; break;}
case 1u:{Il^=0x55555555u; break;}
case 2u:{lI=Il; break;}
default:{break;}
}
if/**/( (lI!=0u)&&(Il!=0u)){
lI=lI^Il;
}
}
