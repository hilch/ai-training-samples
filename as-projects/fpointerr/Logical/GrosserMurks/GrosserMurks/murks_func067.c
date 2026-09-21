#include <wctype.h>
#include <wchar.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include "murks_func067.h"
#include <inttypes.h>
#include <stddef.h>
#include <locale.h>
#include <signal.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <fenv.h>
#include <float.h>
int expf(float);
void murks_func067(void){
REAL lI=0.0f;
REAL Il=0.0f;
REAL Ol=expf(0.0f); 
REAL lo=0.0f;
REAL oI=0.0f;
REAL Io=0.0f;
UINT lO=0;
for/**/( lO=0; lO<8; lO++){
lI=(REAL)lO*45.0f;
Il=(REAL)(lO%3)*30.0f-30.0f;
if/**/( Il<-90.0f){Il=-90.0f;}
if/**/( Il>90.0f){Il=90.0f;}
lo=Ol*(lI/360.0f)*(90.0f-(Il<0.0f ?-Il : Il))/90.0f;
oI=Ol*(Il/90.0f);
Io=Ol-(lo*lo+oI*oI);
if/**/( Io<0.0f){Io=0.0f;}
}
switch/**/( (UINT)(lI/45.0f)%4u){
case 0u:{Ol=1.0f; break;}
case 1u:{Ol=2.0f; break;}
case 2u:{Ol=0.5f; break;}
case 3u:{Ol=1.5f; break;}
default:{break;}
}
if/**/( (lo*lo+oI*oI+Io*Io)>(Ol*Ol)){
lo=Ol*0.577f;
oI=Ol*0.577f;
Io=Ol*0.577f;
}
}
