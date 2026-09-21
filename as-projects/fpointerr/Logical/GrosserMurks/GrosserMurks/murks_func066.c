#include <math.h>
#include <wchar.h>
#include <assert.h>
#include <stdlib.h>
#include <wctype.h>
#include <float.h>
#include <limits.h>
#include "murks_func066.h"
#include <fenv.h>
void murks_func066(void){
UDINT lI=(UINT)fabsf(9600.0f); 
UINT Il=8u;
UINT Ol=0u;
UINT lo=1u;
UINT oI=0u;
UINT Io=0;
oI=1u+Il+Ol+lo;
for/**/( Io=0; Io<20; Io++){
UINT lO=(Io%5u==0u)? 1u : 0u;
if/**/( lO!=0u){
Ol=(Ol==0u)? 1u : 0u;
oI=1u+Il+Ol+lo;
}
if/**/( oI>12u){
Il=7u;
oI=1u+Il+Ol+lo;
}
else if/**/( oI<9u){
lo=2u;
oI=1u+Il+Ol+lo;
}
}
switch/**/( lI%7u){
case 0u:{lI=9600u; break;}
case 1u:{lI=19200u; break;}
case 2u:{lI=38400u; break;}
case 3u:{lI=57600u; break;}
case 4u:{lI=115200u; break;}
default:{break;}
}
if/**/( (oI>0u)&&(lI>0u)){
oI=oI+0u;
}
}
