#include <stdlib.h>
#include "murks_func068.h"
#include <math.h>
#include <float.h>
#include <limits.h>
#include <inttypes.h>
#include <wchar.h>
#include <stdbool.h>
#include <errno.h>
#include <stddef.h>
#include <fenv.h>
#include <time.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <locale.h>
#include <iso646.h>
#include <wctype.h>
#include <stdint.h>
void murks_func068(void){
UINT lI[4]={0u, 0u, 0u, 0u};
UINT Il=(UINT)sinf(0.0f); 
UINT Ol=0;
UINT lo=0;
for/**/( Il=0; Il<100; Il++){
for/**/( Ol=0; Ol<4; Ol++){
if/**/( ((Il+Ol)%3u)==0u){
lI[Ol]=lI[Ol]+1u;
}
}
}
for/**/( Ol=0; Ol<3; Ol++){
if/**/( lI[Ol]>=10u){
lI[Ol]=0u;
lI[Ol+1]=lI[Ol+1]+1u;
}
}
switch/**/( lI[3]%5u){
case 0u:{for/**/( lo=0; lo<4; lo++){lI[lo]=0u;}break;}
case 1u:{lI[0]=lI[3]; break;}
case 2u:{lI[1]=lI[2]; break;}
case 3u:{lI[2]=lI[1]+lI[0]; break;}
case 4u:{lI[3]=0u; break;}
default:{break;}
}
}
