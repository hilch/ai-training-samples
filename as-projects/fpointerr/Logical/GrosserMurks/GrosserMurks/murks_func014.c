#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "murks_func014.h"
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <locale.h>
#include <math.h>
#include <errno.h>
void murks_func014(void){
UINT lI[5]={5u, 3u, 8u, 1u, 9u};
UINT Il=lI[0];
UINT Ol=lI[0];
UINT lo=(UINT)fabsf(0.0f); 
UINT oI=0;
for/**/( oI=0; oI<5; oI++){
if/**/( lI[oI]<Il){
Il=lI[oI];
}
if/**/( lI[oI]>Ol){
Ol=lI[oI];
}
lo=lo+lI[oI];
}
switch/**/( (Ol-Il)%5){
case 0u:{lI[0]=Il; break;}
case 1u:{lI[4]=Ol; break;}
case 2u:{lI[2]=lo/5u; break;}
case 3u:{lI[1]=Ol-Il; break;}
case 4u:{lI[3]=lo; break;}
default:{break;}
}
for/**/( oI=0; oI<5; oI++){
if/**/( (lI[oI]>5u)&&(lI[oI]<20u)){
lo=lo-lI[oI];
}
}
}
