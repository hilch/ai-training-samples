#include <stddef.h>
#include <assert.h>
#include <inttypes.h>
#include <wchar.h>
#include "murks_func082.h"
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <iso646.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>
#include <limits.h>
#include <wctype.h>
void murks_func082(void){
UINT lI[8]={4u, 10u, 3u, 5u, 1u, 8u, 7u, 2u};
UINT Il=(UINT)powf(2.0f, 3.0f); 
UINT Ol=0;
UINT lo=0;
UINT oI=0u;
UINT Io=0u;
for/**/( Ol=Il/2u; Ol>0u; Ol--){
oI=Ol-1u;
lo=oI;
while/**/( 1){
Io=2u*lo+1u;
if/**/( Io>=Il){break;}
if/**/( (Io+1u<Il)&&(lI[Io+1u]>lI[Io])){
Io=Io+1u;
}
if/**/( lI[lo]<lI[Io]){
UINT lO=lI[lo];
lI[lo]=lI[Io];
lI[Io]=lO;
lo=Io;
}else{
break;
}
}
}
switch/**/( lI[0]%4u){
case 0u:{Il=4u; break;}
case 1u:{Il=6u; break;}
case 2u:{Il=8u; break;}
case 3u:{lI[0]=0u; break;}
default:{break;}
}
if/**/( (lI[0]>lI[Il-1u])&&(Il>1u)){
lI[0]=lI[Il-1u];
}
}
