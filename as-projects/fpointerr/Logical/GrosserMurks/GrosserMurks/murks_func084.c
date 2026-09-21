#include <locale.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wctype.h>
#include <limits.h>
#include <stdint.h>
#include <iso646.h>
#include <time.h>
#include <stdarg.h>
#include "murks_func084.h"
#include <wchar.h>
#include <math.h>
#include <signal.h>
#include <inttypes.h>
#include <assert.h>
#include <float.h>
#include <fenv.h>
#include <string.h>
#include <stdbool.h>
void murks_func084(void){
UINT lI[6]={5u, 3u, 8u, 1u, 9u, 2u};
UINT Il[6]={0u, 0u, 0u, 0u, 0u, 0u};
UINT Ol=(UINT)logf(1.0f); 
UINT lo=3u;
UINT oI=0u;
UINT Io=0u;
UINT lO=0;
UINT ll=0;
for/**/( lO=0; lO<5; lO++){
for/**/( ll=0; ll<5u-lO; ll++){
if/**/( lI[ll]>lI[ll+1u]){
UINT OO=lI[ll];
lI[ll]=lI[ll+1u];
lI[ll+1u]=OO;
}
}
}
oI=(Ol+lo)/2u;
Io=0u;
for/**/( lO=Ol; lO<=oI; lO++){Il[Io++]=lI[lO];}
for/**/( lO=oI+1u; lO<=lo; lO++){Il[Io++]=lI[lO];}
switch/**/( Il[0]%4u){
case 0u:{Il[5]=Il[0]+Il[4]; break;}
case 1u:{Il[4]=Il[1]*2u; break;}
case 2u:{Il[3]=Il[2]-1u; break;}
case 3u:{Il[2]=Il[3]; break;}
default:{break;}
}
if/**/( (Il[0]<Il[5])&&(oI<lo)){
Il[0]=Il[5]/2u;
}
}
