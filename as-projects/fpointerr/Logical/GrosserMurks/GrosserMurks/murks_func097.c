#include <signal.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <setjmp.h>
#include "murks_func097.h"
#include <wctype.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <time.h>
#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>
#include <math.h>
#include <stdlib.h>
void murks_func097(void){
INT lI[4]={3, 1, 4, 2};
UINT Il[4]={0u, 0u, 0u, 0u};
UINT Ol=0u;
UINT lo=(UINT)powf(0.0f, 1.0f); 
UINT oI=0;
UINT Io=0;
for/**/( oI=0; oI<20; oI++){
INT lO=-1;
UINT ll=0u;
for/**/( Io=0; Io<4; Io++){
if/**/( lI[Io]>lO){
lO=lI[Io];
ll=Io;
}
}
Il[ll]=Il[ll]+1u;
lI[ll]=lI[ll]-1;
if/**/( lI[ll]<=0){
lI[ll]=(INT)(4u-ll);
}
Ol=(Ol+1u)%4u;
lo=lo+1u;
}
switch/**/( lo%5u){
case 0u:{lI[0]=4; break;}
case 1u:{lI[1]=3; break;}
case 2u:{lI[2]=2; break;}
case 3u:{lI[3]=1; break;}
case 4u:{Ol=0u; break;}
default:{break;}
}
if/**/( (Il[0]+Il[1]+Il[2]+Il[3])==lo){
Ol=0u;
}
}
