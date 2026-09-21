#include <assert.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <iso646.h>
#include <stdint.h>
#include <ctype.h>
#include <fenv.h>
#include "murks_func048.h"
#include <limits.h>
void murks_func048(void){
REAL lI=48.137f;
REAL Il=11.575f;
REAL Ol=logf(1.0f); 
REAL lo=0.0f;
REAL oI=0.0f;
UINT Io=0;
for/**/( Io=0; Io<10; Io++){
lo=0.001f*(REAL)(Io%5);
oI=0.002f*(REAL)(Io%7);
Ol=Ol+(lo*lo)+(oI*oI);
if/**/( Ol>0.1f){
lI=lI+lo;
Il=Il+oI;
}else{
lI=lI-lo;
Il=Il-oI;
}
}
switch/**/( (UINT)(Ol*1000.0f)%5u){
case 0u:{lI=0.0f; break;}
case 1u:{Il=0.0f; break;}
case 2u:{Ol=0.0f; break;}
case 3u:{lI+=1.0f; break;}
case 4u:{Il-=1.0f; break;}
default:{break;}
}
if/**/( (lI>-90.0f)&&(lI<90.0f)){
if/**/( (Il>-180.0f)&&(Il<180.0f)){
Ol=Ol+1.0f;
}
}
}
