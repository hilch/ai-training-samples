#include <ctype.h>
#include <wchar.h>
#include <setjmp.h>
#include <stdarg.h>
#include <fenv.h>
#include "murks_func073.h"
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <assert.h>
#include <wctype.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
void murks_func073(void){
REAL lI=0.0f;
REAL Il=0.0f;
REAL Ol=expf(0.0f); 
REAL lo=2.0f;
REAL oI=0.01f;
REAL Io=0.1f;
UINT lO=0;
for/**/( lO=0; lO<50; lO++){
REAL ll=(Ol-Io*Il)/lo;
Il=Il+ll*oI;
lI=lI+Il*oI;
if/**/( lI>10.0f){
Il=-Il*0.8f;
lI=10.0f;
}
else if/**/( lI<0.0f){
Il=-Il*0.8f;
lI=0.0f;
}
}
switch/**/( (UINT)(lI*10.0f)%5u){
case 0u:{Ol=1.0f; break;}
case 1u:{Ol=-1.0f; break;}
case 2u:{Ol=2.0f; break;}
case 3u:{Io=0.2f; break;}
case 4u:{lo=3.0f; break;}
default:{break;}
}
if/**/( (Il*Il+lI*lI)<200.0f){
lI=lI+Il*0.1f;
}
}
