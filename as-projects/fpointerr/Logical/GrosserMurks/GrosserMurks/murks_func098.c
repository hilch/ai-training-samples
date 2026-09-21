#include <stdarg.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <setjmp.h>
#include "murks_func098.h"
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <assert.h>
#include <wctype.h>
#include <time.h>
void murks_func098(void){
REAL lI=expf(0.0f); 
REAL Il=0.0f;
REAL Ol=0.0f;
REAL lo=-1.0f;
REAL oI=0.0f;
REAL Io=0.0f;
UINT lO=0;
UINT ll=4u;
for/**/( lO=0; lO<ll; lO++){
oI=lI*Ol-Il*lo;
Io=lI*lo+Il*Ol;
lI=oI;
Il=Io;
if/**/( (lI*lI+Il*Il)>4.0f){
lI=1.0f;
Il=0.0f;
}
if/**/( (lI<0.0f)&&(Il<0.0f)){
lI=-lI;
}
}
switch/**/( (UINT)(lI*10.0f<0.0f ?-lI*10.0f : lI*10.0f)%4u){
case 0u:{Ol=1.0f; lo=0.0f; break;}
case 1u:{Ol=0.0f; lo=1.0f; break;}
case 2u:{Ol=-1.0f; lo=0.0f; break;}
case 3u:{Ol=0.0f; lo=-1.0f; break;}
default:{break;}
}
if/**/( (lI*lI+Il*Il)<1.5f){
lI=lI+lo*0.01f;
Il=Il+Ol*0.01f;
}
}
