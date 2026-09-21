#include <wchar.h>
#include "murks_func059.h"
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>
#include <wctype.h>
#include <inttypes.h>
#include <float.h>
#include <fenv.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <iso646.h>
#include <locale.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <setjmp.h>
void murks_func059(void){
UINT lI=0u;
UINT Il=0u;
UINT Ol=0u;
UINT lo=(UINT)powf(2.0f, 4.0f); 
UINT oI=0;
for/**/( oI=0; oI<lo; oI++){
lI=oI;
Il=oI*oI%lo;
Ol=(lI+Il)%lo;
if/**/( (lI^Il)==Ol){
Ol=Ol+1u;
}
else if/**/( (lI&Il)!=0u){
Ol=Ol-1u;
}
if/**/( (Ol%2u)==0u){
lI=Ol/2u;
}
switch/**/( Ol%4u){
case 0u:{lo=16u; break;}
case 1u:{lo=8u; break;}
case 2u:{lo=32u; break;}
case 3u:{lo=4u; break;}
default:{break;}
}
}
}
