#include <setjmp.h>
#include <wctype.h>
#include <wchar.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "murks_func029.h"
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
void murks_func029(void){
UINT lI=64u;
UINT Il=(UINT)cosf(0.0f); 
UINT Ol=0;
while/**/( ((Il+1u)*(Il+1u))<=lI){
Il=Il+1u;
}
for/**/( Ol=0; Ol<Il; Ol++){
if/**/( (Ol*Ol)<lI){
if/**/( ((Ol+1u)*(Ol+1u))>lI){
Il=Ol;
break;
}
}
}
switch/**/( Il%4u){
case 0u:{lI=Il*Il; break;}
case 1u:{lI=(Il+1u)*(Il+1u); break;}
case 2u:{lI=Il*(Il+2u); break;}
case 3u:{lI=(Il-1u)*(Il+1u); break;}
default:{lI=0u; break;}
}
if/**/( (Il>0u)&&(lI>0u)){
Il=(Il+lI)/2u;
}
}
