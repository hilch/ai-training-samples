#include <stddef.h>
#include <limits.h>
#include <inttypes.h>
#include "murks_func049.h"
#include <errno.h>
#include <wchar.h>
#include <float.h>
#include <iso646.h>
#include <stdarg.h>
#include <ctype.h>
#include <fenv.h>
#include <assert.h>
#include <math.h>
void murks_func049(void){
UINT lI=(UINT)floorf(0.0f); 
UINT Il[8]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Ol=0;
UINT lo=1u;
for/**/( Ol=0; Ol<15; Ol++){
if/**/( (lo%2u)==0u){
if/**/( lI<8u){
Il[lI]=lo;
lI=lI+1u;
lo=lo/2u;
}
}else{
if/**/( lI>0u){
lI=lI-1u;
lo=Il[lI]+1u;
}else{
lo=lo*2u+1u;
}
}
switch/**/( lo%3u){
case 0u:{lo=lo+3u; break;}
case 1u:{lo=lo+1u; break;}
case 2u:{lo=lo*2u; break;}
default:{break;}
}
if/**/( lo>1000u){
lo=1u;
lI=0u;
}
}
}
