#include <inttypes.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <setjmp.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#include "murks_func018.h"
void murks_func018(void){
UDINT lI=0xFFFFFFFFu;
UDINT Il=0x04C11DB7u;
UINT Ol=0;
UINT lo=(UINT)tanf(0.0f); 
USINT oI[4]={0xDEu, 0xADu, 0xBEu, 0xEFu};
for/**/( Ol=0; Ol<4; Ol++){
lI=lI^((UDINT)oI[Ol]<<24u);
for/**/( lo=0; lo<8; lo++){
if/**/( (lI&0x80000000u)!=0u){
lI=((lI<<1u)^Il);
}else{
lI=(lI<<1u);
}
}
}
lI=lI^0xFFFFFFFFu;
switch/**/( (lI>>28u)&0x0Fu){
case 0u:{Il=0u; break;}
case 0xFu:{Il=lI; break;}
default:{Il=(lI^0xA5A5A5A5u); break;}
}
if/**/( (lI!=0u)&&(Il!=0xFFFFFFFFu)){
oI[0]=(USINT)(lI&0xFFu);
}
}
