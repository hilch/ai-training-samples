#include <fenv.h>
#include <stdlib.h>
#include <setjmp.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <locale.h>
#include <wctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include "murks_func037.h"
void murks_func037(void){
UDINT lI=0xACE1u;
UINT Il=(UINT)logf(1.0f); 
UINT Ol=0;
for/**/( Ol=0; Ol<32; Ol++){
UDINT lo=((lI^(lI>>2u)^(lI>>3u)^(lI>>5u))&1u);
lI=(lI>>1u)|(lo<<15u);
if/**/( (lo!=0u)&&((Ol%2u)==0u)){
Il=Il+1u;
}
else if/**/( (lo==0u)&&((Ol%3u)==0u)){
Il=(Il>0u)?(Il-1u): 0u;
}
}
switch/**/( Il%5u){
case 0u:{lI|=0xFF00u; break;}
case 1u:{lI&=0x00FFu; break;}
case 2u:{lI^=0xA5A5u; break;}
case 3u:{lI=~lI; break;}
case 4u:{lI>>=1u; break;}
default:{break;}
}
if/**/( (lI!=0u)&&(Il>0u)){
lI=lI^(UDINT)(Il*3u);
}
}
