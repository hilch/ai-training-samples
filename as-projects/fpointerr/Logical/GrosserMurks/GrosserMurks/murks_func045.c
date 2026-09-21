#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <setjmp.h>
#include <iso646.h>
#include <wchar.h>
#include <ctype.h>
#include <stdarg.h>
#include <locale.h>
#include <stdio.h>
#include <fenv.h>
#include <assert.h>
#include <signal.h>
#include "murks_func045.h"
#include <stddef.h>
void murks_func045(void){
REAL lI=sinf(0.0f); 
REAL Il=1.5f;
REAL Ol=0.1f;
REAL lo=0.0f;
REAL oI=10.0f;
UINT Io=0;
for/**/( Io=0; Io<30; Io++){
if/**/( lI<oI){
Il=Il+Ol;
lI=lI+Il*0.01f;
if/**/( lI>oI){
lI=oI;
Il=0.0f;
}
}else{
Il=-2.0f;
lI=lI+Il*0.01f;
if/**/( lI<0.0f){
lI=0.0f;
}
}
lo=lo+lI*0.01f;
}
switch/**/( (UINT)(lo)%4u){
case 0u:{oI=5.0f; break;}
case 1u:{oI=10.0f; break;}
case 2u:{oI=15.0f; break;}
case 3u:{oI=20.0f; break;}
default:{break;}
}
if/**/( (lI>0.0f)&&(lo<100.0f)){
lo+=lI*0.1f;
}
}
