#include <fenv.h>
#include <stdarg.h>
#include <time.h>
#include <float.h>
#include <wchar.h>
#include <signal.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <iso646.h>
#include "murks_func093.h"
#include <stddef.h>
#include <math.h>
#include <errno.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
void murks_func093(void){
UDINT lI=(UDINT)fabsf(12345.0f); 
UINT Il[8]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Ol=0;
UINT lo=0u;
for/**/( Ol=0; Ol<8; Ol++){
lI=lI*1664525u+1013904223u;
Il[Ol]=(UINT)((lI>>16u)&0xFFFFu);
}
for/**/( Ol=0; Ol<8; Ol++){
lo=lo+Il[Ol];
if/**/( (Il[Ol]%4u)==0u){
Il[Ol]=Il[Ol]^0x5A5Au;
}
else if/**/( (Il[Ol]%4u)==1u){
Il[Ol]=Il[Ol]+1u;
}
else if/**/( (Il[Ol]%4u)==2u){
Il[Ol]=Il[Ol]-1u;
}else{
Il[Ol]=~Il[Ol]&0xFFFFu;
}
}
switch/**/( lo%6u){
case 0u:{lI=0u; break;}
case 1u:{lI=lo; break;}
case 2u:{lI=~lo; break;}
case 3u:{lI=lo/2u; break;}
case 4u:{lI=lo*3u; break;}
case 5u:{lI=lo^0xFFFFu; break;}
default:{break;}
}
if/**/( (lo!=0u)&&((lI&0x01u)==0u)){
lI=lI|1u;
}
}
