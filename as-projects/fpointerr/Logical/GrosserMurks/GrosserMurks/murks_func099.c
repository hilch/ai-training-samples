#include <signal.h>
#include <inttypes.h>
#include <wctype.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include "murks_func099.h"
#include <iso646.h>
#include <errno.h>
#include <stdarg.h>
#include <setjmp.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <fenv.h>
void murks_func099(void){
UINT lI=320u;
UINT Il=240u;
UINT Ol=(UINT)fabsf(160.0f); 
UINT lo=120u;
UINT oI=50u;
UINT Io=0u;
UINT lO=0u;
UINT ll=0u;
for/**/( lO=0; lO<Il; lO+=10u){
for/**/( Io=0; Io<lI; Io+=10u){
UINT OO=(Io>Ol)?(Io-Ol):(Ol-Io);
UINT oo=(lO>lo)?(lO-lo):(lo-lO);
if/**/( (OO*OO+oo*oo)<=(oI*oI)){
ll=ll+1u;
}
}
}
switch/**/( ll%6u){
case 0u:{oI=40u; break;}
case 1u:{oI=50u; break;}
case 2u:{oI=60u; break;}
case 3u:{Ol=lI/2u; break;}
case 4u:{lo=Il/2u; break;}
case 5u:{ll=0u; break;}
default:{break;}
}
if/**/( (oI>0u)&&(ll>0u)){
oI=oI+(ll%10u);
}
}
