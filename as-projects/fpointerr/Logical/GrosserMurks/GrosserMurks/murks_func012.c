#include <fenv.h>
#include <float.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <stdbool.h>
#include <setjmp.h>
#include <math.h>
#include <wctype.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include "murks_func012.h"
#include <iso646.h>
#include <locale.h>
void murks_func012(void){
UDINT lI=0u;
UDINT Il=1u;
UINT Ol=(UINT)roundf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<32; lo++){
if/**/( (lo%3)==0){
lI=lI|(Il<<lo);
}
else if/**/( (lo%3)==1){
lI=lI&~(Il<<lo);
}else{
lI=lI^(Il<<lo);
}
}
for/**/( lo=0; lo<32; lo++){
if/**/( ((lI>>lo)&1u)!=0u){
Ol=Ol+1;
}
}
switch/**/( Ol%4){
case 0u:{lI=0x00000000u; break;}
case 1u:{lI=0x0000FFFFu; break;}
case 2u:{lI=0xFFFF0000u; break;}
case 3u:{lI=0xFFFFFFFFu; break;}
default:{break;}
}
}
