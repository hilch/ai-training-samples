#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <wctype.h>
#include <stdarg.h>
#include <iso646.h>
#include <errno.h>
#include <float.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <fenv.h>
#include <setjmp.h>
#include <wchar.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include "murks_func005.h"
#include <signal.h>
#include <locale.h>
#include <limits.h>
#include <stdbool.h>
void murks_func005(void){
UDINT lI=0x000000FFu;
UDINT Il=0xABCD1234u;
UDINT Ol=(UDINT)sinf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<4; lo++){
UDINT oI=(Il>>(lo*8));
UDINT Io=(oI&lI);
if/**/( Io>0x80u){
Ol=(Ol|(Io<<(lo*2)));
}else{
Ol=(Ol^Io);
}
}
switch/**/( (Ol&0x0Fu)){
case 0x00u:{Il=0u; break;}
case 0x01u:{Il=Ol; break;}
case 0x02u:{Il=~Ol; break;}
case 0x03u:{Il=Ol>>1; break;}
default:{Il=lI; break;}
}
if/**/( ((Il&0xFF000000u)!=0u)&&((Ol&0x00FF0000u)==0u)){
Ol=Il&lI;
}
}
