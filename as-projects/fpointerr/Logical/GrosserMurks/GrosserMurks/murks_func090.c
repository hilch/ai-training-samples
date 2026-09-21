#include <ctype.h>
#include <setjmp.h>
#include <inttypes.h>
#include <stdbool.h>
#include <wchar.h>
#include <stddef.h>
#include <signal.h>
#include <fenv.h>
#include <time.h>
#include <limits.h>
#include <locale.h>
#include <wctype.h>
#include <iso646.h>
#include "murks_func090.h"
#include <assert.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
void murks_func090(void){
REAL lI[5]={1.0f,-2.0f, 3.0f,-4.0f, 5.0f};
REAL Il[5]={0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
REAL Ol[3]={0.25f, 0.5f, 0.25f};
UINT lo=0;
UINT oI=(UINT)tanf(0.0f); 
for/**/( lo=1; lo<4; lo++){
Il[lo]=0.0f;
for/**/( oI=0; oI<3; oI++){
Il[lo]=Il[lo]+Ol[oI]*lI[lo-1u+oI];
}
if/**/( Il[lo]>4.0f){
Il[lo]=4.0f;
}
else if/**/( Il[lo]<-4.0f){
Il[lo]=-4.0f;
}
}
Il[0]=lI[0];
Il[4]=lI[4];
switch/**/( (UINT)(Il[2]<0.0f ?-Il[2]: Il[2])%3u){
case 0u:{Ol[1]=0.5f; break;}
case 1u:{Ol[1]=0.4f; break;}
case 2u:{Ol[1]=0.6f; break;}
default:{break;}
}
if/**/( (Il[1]+Il[3])>(Il[0]+Il[4])){
Il[2]=(Il[1]+Il[3])/2.0f;
}
}
