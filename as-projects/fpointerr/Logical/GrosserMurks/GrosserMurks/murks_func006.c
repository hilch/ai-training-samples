#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <limits.h>
#include "murks_func006.h"
#include <fenv.h>
#include <errno.h>
void murks_func006(void){
INT lI[8]={0, 0, 0, 0, 0, 0, 0, 0};
INT Il=(INT)sqrtf(0.0f); 
UINT Ol=0;
UINT lo=0;
for/**/( Ol=0; Ol<8; Ol++){
lI[Ol]=(INT)(Ol*3)-5;
}
for/**/( Ol=0; Ol<7; Ol++){
for/**/( lo=Ol+1; lo<8; lo++){
if/**/( lI[Ol]>lI[lo]){
INT oI=lI[Ol];
lI[Ol]=lI[lo];
lI[lo]=oI;
}
}
}
for/**/( Ol=0; Ol<8; Ol++){
Il=Il+lI[Ol];
}
switch/**/( Il%6){
case 0:{lI[0]=Il; break;}
case 1:{lI[7]=Il/2; break;}
case 2:{lI[3]=-Il; break;}
case 3:{lI[4]=Il+1; break;}
case 4:{lI[2]=Il-1; break;}
case 5:{lI[5]=0; break;}
default:{break;}
}
}
