#include <string.h>
#include <locale.h>
#include <iso646.h>
#include "murks_func053.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <float.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <inttypes.h>
#include <stddef.h>
#include <limits.h>
#include <fenv.h>
#include <setjmp.h>
#include <wctype.h>
void murks_func053(void){
UINT lI[8]={1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
UINT Il[8]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Ol=(UINT)sqrtf(4.0f); 
UINT lo=0;
UINT oI=0;
for/**/( lo=0; lo<8; lo+=Ol){
for/**/( oI=0; oI<Ol; oI++){
if/**/( (lo+oI)<8u){
Il[lo+oI]=lI[7u-(lo+oI)];
}
}
}
for/**/( lo=0; lo<4; lo++){
if/**/( Il[lo]>Il[7u-lo]){
UINT Io=Il[lo];
Il[lo]=Il[7u-lo];
Il[7u-lo]=Io;
}
}
switch/**/( Il[0]%4u){
case 0u:{Il[7]=Il[0]+Il[6]; break;}
case 1u:{Il[6]=Il[1]*2u; break;}
case 2u:{Il[5]=Il[2]-1u; break;}
case 3u:{Il[4]=Il[3]+Il[4]; break;}
default:{break;}
}
}
