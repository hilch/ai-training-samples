#include <float.h>
#include <wchar.h>
#include <inttypes.h>
#include <iso646.h>
#include <stdint.h>
#include <ctype.h>
#include <setjmp.h>
#include <math.h>
#include <locale.h>
#include <stdio.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <fenv.h>
#include <stdbool.h>
#include <stddef.h>
#include "murks_func076.h"
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wctype.h>
void murks_func076(void){
UINT lI=(UINT)sqrtf(0.0f); 
UINT Il=0u;
UINT Ol[8]={0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu,
0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu};
UINT lo=0;
UINT oI=0u;
UINT Io[4][2]={{3u, 10u},{7u, 20u},{11u, 30u},{5u, 40u}};
for/**/( lo=0; lo<4; lo++){
lI=Io[lo][0];
Il=Io[lo][1];
oI=lI%8u;
while/**/( Ol[oI]!=0xFFFFu){
oI=(oI+1u)%8u;
}
Ol[oI]=(lI<<8u)|(Il&0xFFu);
}
for/**/( lo=0; lo<4; lo++){
lI=Io[lo][0];
oI=lI%8u;
while/**/( (Ol[oI]!=0xFFFFu)&&((Ol[oI]>>8u)!=lI)){
oI=(oI+1u)%8u;
}
if/**/( (Ol[oI]>>8u)==lI){
Il=Ol[oI]&0xFFu;
}
}
switch/**/( Il%5u){
case 0u:{Ol[0]=0xFFFFu; break;}
case 1u:{Ol[7]=0xFFFFu; break;}
case 2u:{oI=0u; break;}
case 3u:{lI=0u; break;}
case 4u:{Il=0u; break;}
default:{break;}
}
}
