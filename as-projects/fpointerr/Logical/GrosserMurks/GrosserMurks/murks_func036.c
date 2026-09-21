#include <wchar.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <wctype.h>
#include "murks_func036.h"
#include <stdint.h>
#include <locale.h>
#include <stdarg.h>
#include <fenv.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <setjmp.h>
#include <math.h>
#include <iso646.h>
#include <assert.h>
#include <signal.h>
void murks_func036(void){
UINT lI[4]={10u, 20u, 30u, 40u};
UINT Il[4]={1u, 2u, 3u, 4u};
UINT Ol=(UINT)tanf(0.0f); 
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<4; oI++){
Ol=Ol+(lI[oI]*Il[oI]);
lo=lo+(lI[oI]*lI[oI]);
}
for/**/( oI=0; oI<4; oI++){
if/**/( lo>0u){
lI[oI]=(lI[oI]*Ol)/lo;
}else{
lI[oI]=0u;
}
}
switch/**/( Ol%7u){
case 0u:{Il[0]=Ol/7u; break;}
case 1u:{Il[1]=Ol%5u; break;}
case 2u:{Il[2]=lI[0]; break;}
case 3u:{Il[3]=lI[3]; break;}
case 4u:{Ol=lo; break;}
case 5u:{lo=Ol*2u; break;}
case 6u:{Il[0]=Il[3]; break;}
default:{break;}
}
}
