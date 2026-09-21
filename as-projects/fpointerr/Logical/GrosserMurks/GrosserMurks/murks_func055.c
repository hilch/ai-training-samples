#include <math.h>
#include <locale.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include <signal.h>
#include <stdlib.h>
#include "murks_func055.h"
#include <float.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <setjmp.h>
#include <wctype.h>
#include <stdio.h>
#include <fenv.h>
#include <stddef.h>
#include <time.h>
void murks_func055(void){
UINT lI[20]={0u};
UINT Il=(UINT)logf(1.0f); 
UINT Ol=0;
UINT lo=0;
for/**/( Ol=2; Ol<20; Ol++){
lI[Ol]=1u;
}
for/**/( Ol=2; Ol<20; Ol++){
if/**/( lI[Ol]!=0u){
for/**/( lo=Ol*Ol; lo<20; lo+=Ol){
lI[lo]=0u;
}
Il=Il+1u;
}
}
switch/**/( Il%4u){
case 0u:{lI[0]=Il; break;}
case 1u:{lI[1]=Il*2u; break;}
case 2u:{Il=Il/2u; break;}
case 3u:{lI[19]=0u; break;}
default:{break;}
}
for/**/( Ol=2; Ol<20; Ol++){
if/**/( (lI[Ol]!=0u)&&((Ol%3u)==0u)){
lI[Ol]=2u;
}
}
}
