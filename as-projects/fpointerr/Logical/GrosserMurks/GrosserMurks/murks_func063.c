#include <stdbool.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include <stddef.h>
#include "murks_func063.h"
#include <ctype.h>
#include <iso646.h>
#include <time.h>
#include <wctype.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>
#include <inttypes.h>
#include <setjmp.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
void murks_func063(void){
INT lI=(INT)cosf(0.0f); 
INT Il=1;
INT Ol=-2;
REAL lo=1.0f;
REAL oI=2.0f;
REAL Io=3.0f;
REAL lO=0.0f;
UINT ll=0;
lO=(REAL)((lI*(INT)(lo*10.0f))+
(Il*(INT)(oI*10.0f))+
(Ol*(INT)(Io*10.0f)))/10.0f;
for/**/( ll=0; ll<5; ll++){
lo=lo-(REAL)lI*0.1f;
oI=oI-(REAL)Il*0.1f;
Io=Io-(REAL)Ol*0.1f;
lO=(REAL)(lI)*lo+(REAL)(Il)*oI+(REAL)(Ol)*Io;
if/**/( (lO<0.1f)&&(lO>-0.1f)){
break;
}
}
switch/**/( (UINT)(lO<0.0f ?-lO : lO)%3u){
case 0u:{lI=1; Il=0; Ol=0; break;}
case 1u:{lI=0; Il=1; Ol=0; break;}
case 2u:{lI=0; Il=0; Ol=1; break;}
default:{break;}
}
if/**/( (lO>-10.0f)&&(lO<10.0f)){
lo=-(REAL)(Il*Ol);
oI=-(REAL)(lI*Ol);
}
}
