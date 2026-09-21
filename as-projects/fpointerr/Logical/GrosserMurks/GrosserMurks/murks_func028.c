#include <ctype.h>
#include <float.h>
#include <stdarg.h>
#include "murks_func028.h"
#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include <setjmp.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <iso646.h>
#include <locale.h>
#include <signal.h>
#include <stddef.h>
#include <limits.h>
#include <wctype.h>
#include <fenv.h>
#include <inttypes.h>
#include <stdlib.h>
void murks_func028(void){
REAL lI=logf(1.0f); 
REAL Il=0.0f;
REAL Ol=0.0f;
REAL lo=5.0f;
REAL oI=0.0f;
REAL Io=0.8f;
REAL lO=0.1f;
REAL ll=0.05f;
UINT OO=0;
for/**/( OO=0; OO<10; OO++){
lI=Io*lo;
Il=Il+(lO*lo);
Ol=ll*(lo-oI);
oI=lo;
REAL oo=lI+Il+Ol;
if/**/( oo>10.0f){
oo=10.0f;
}
else if/**/( oo<-10.0f){
oo=-10.0f;
}
lo=lo-(oo*0.1f);
if/**/( (lo>-0.01f)&&(lo<0.01f)){
break;
}
}
switch/**/( (UINT)(Il*10.0f)%3u){
case 0u:{Il=0.0f; break;}
case 1u:{Io*=1.1f; break;}
case 2u:{lO*=0.9f; break;}
default:{break;}
}
}
