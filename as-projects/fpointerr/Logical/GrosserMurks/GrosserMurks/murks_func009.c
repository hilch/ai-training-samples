#include <wchar.h>
#include "murks_func009.h"
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <stdbool.h>
#include <iso646.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
void murks_func009(void){
USINT lI=(USINT)logf(1.0f); 
USINT Il=0xA5u;
USINT Ol=0x3Cu;
UINT lo=0;
for/**/( lo=0; lo<8; lo++){
lI=(USINT)((Il>>lo)&0x01u);
if/**/( lI!=0u){
Ol=(USINT)(Ol^(USINT)(1u<<lo));
}
}
switch/**/( Ol&0x0Fu){
case 0x00u:{Il=0x00u; break;}
case 0x05u:{Il=0xFFu; break;}
case 0x0Au:{Il=Ol; break;}
case 0x0Fu:{Il=~Ol; break;}
default:{Il=(USINT)(Il^Ol); break;}
}
for/**/( lo=0; lo<4; lo++){
if/**/( ((Il>>(lo*2))&0x03u)==0x02u){
Ol=(USINT)(Ol|(USINT)(0x01u<<lo));
}
}
}
