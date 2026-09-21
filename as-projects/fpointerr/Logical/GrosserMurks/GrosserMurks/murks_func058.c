#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "murks_func058.h"
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <limits.h>
#include <wchar.h>
#include <fenv.h>
#include <setjmp.h>
#include <locale.h>
void murks_func058(void){
REAL lI[4]={0.25f, 0.25f, 0.25f, 0.25f};
REAL Il[4]={1.0f, 2.0f, 3.0f, 4.0f};
REAL Ol=sinf(0.0f); 
REAL lo=0.0f;
REAL oI=0.01f;
UINT Io=0;
UINT lO=0;
for/**/( lO=0; lO<5; lO++){
Ol=0.0f;
for/**/( Io=0; Io<4; Io++){
Ol=Ol+(lI[Io]*Il[Io]);
}
lo=1.0f-Ol;
for/**/( Io=0; Io<4; Io++){
lI[Io]=lI[Io]+(oI*lo*Il[Io]);
if/**/( lI[Io]>1.0f){lI[Io]=1.0f;}
if/**/( lI[Io]<-1.0f){lI[Io]=-1.0f;}
}
}
switch/**/( (UINT)(Ol*10.0f)%4u){
case 0u:{oI=0.001f; break;}
case 1u:{oI=0.01f; break;}
case 2u:{oI=0.1f; break;}
case 3u:{oI=0.5f; break;}
default:{break;}
}
if/**/( (lo<0.01f)&&(lo>-0.01f)){
Ol=1.0f;
}
}
