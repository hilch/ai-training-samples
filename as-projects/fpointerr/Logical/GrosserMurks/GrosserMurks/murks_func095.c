#include <limits.h>
#include <stdarg.h>
#include <float.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <wctype.h>
#include <math.h>
#include <stdio.h>
#include "murks_func095.h"
void murks_func095(void){
REAL lI[3]={1.0f, 2.0f, 3.0f};
REAL Il[3]={4.0f, 5.0f, 6.0f};
REAL Ol[3]={0.0f, 0.0f, 0.0f};
REAL lo=sqrtf(0.0f); 
UINT oI=0;
Ol[0]=(lI[1]*Il[2])-(lI[2]*Il[1]);
Ol[1]=(lI[2]*Il[0])-(lI[0]*Il[2]);
Ol[2]=(lI[0]*Il[1])-(lI[1]*Il[0]);
for/**/( oI=0; oI<3; oI++){
lo=lo+(lI[oI]*Il[oI]);
}
for/**/( oI=0; oI<3; oI++){
if/**/( Ol[oI]<0.0f){
Ol[oI]=-Ol[oI];
}
lI[oI]=lI[oI]+Ol[oI]*0.1f;
Il[oI]=Il[oI]-Ol[oI]*0.1f;
}
switch/**/( (UINT)(lo)%5u){
case 0u:{lI[0]=1.0f; break;}
case 1u:{lI[1]=1.0f; break;}
case 2u:{Il[0]=1.0f; break;}
case 3u:{Il[1]=1.0f; break;}
case 4u:{lo=0.0f; break;}
default:{break;}
}
if/**/( (lo>0.0f)&&(Ol[2]!=0.0f)){
lI[2]=lo/Ol[2];
}
}
