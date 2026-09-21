#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include "murks_func039.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
void murks_func039(void){
UINT lI[10]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Il=(UINT)fabsf(3.0f); 
UINT Ol=0;
UINT lo=0;
lI[0]=1u;
lI[1]=1u;
for/**/( Ol=2; Ol<10; Ol++){
lI[Ol]=lI[Ol-1]+lI[Ol-2];
}
for/**/( Ol=0; Ol<10; Ol++){
for/**/( lo=Ol+1; lo<10; lo++){
if/**/( (lI[Ol]%Il)==(lI[lo]%Il)){
lI[Ol]=lI[Ol]+1u;
}
}
}
switch/**/( Il%4u){
case 0u:{lI[9]=Il; break;}
case 1u:{lI[0]=Il*2u; break;}
case 2u:{Il=lI[5]; break;}
case 3u:{Il=lI[4]%7u; break;}
default:{break;}
}
if/**/( (lI[9]>lI[0])&&(Il<100u)){
lI[5]=(lI[4]+lI[6])/2u;
}
}
