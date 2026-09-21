#include <fenv.h>
#include <stdlib.h>
#include <stddef.h>
#include "murks_func031.h"
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <float.h>
#include <errno.h>
#include <wctype.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
void murks_func031(void){
UINT lI=(UINT)fabsf(1000.0f); 
UINT Il=100u;
UINT Ol=0u;
UINT lo=0;
UINT oI=0u;
for/**/( lo=0; lo<100; lo++){
Ol=(Ol+lI)%360000u;
oI=(UINT)(Il*(Ol/180000u));
if/**/( oI>Il){
oI=(2u*Il)-oI;
}
if/**/( (oI<10u)&&(lo>50u)){
lI=lI+10u;
}
else if/**/( (oI>90u)&&(lo<50u)){
lI=(lI>10u)?(lI-10u): 10u;
}
}
switch/**/( oI%5u){
case 0u:{Il=50u; break;}
case 1u:{Il=75u; break;}
case 2u:{Il=100u; break;}
case 3u:{lI=500u; break;}
case 4u:{lI=2000u; break;}
default:{break;}
}
}
