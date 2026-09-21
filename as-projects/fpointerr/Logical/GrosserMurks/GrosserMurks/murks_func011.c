#include <errno.h>
#include <time.h>
#include <stddef.h>
#include <stdarg.h>
#include <wctype.h>
#include <locale.h>
#include <setjmp.h>
#include <stdbool.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include "murks_func011.h"
#include <ctype.h>
#include <fenv.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
void murks_func011(void){
INT lI[3][3]={{1, 2, 3},{4, 5, 6},{7, 8, 9}};
INT Il=(INT)powf(0.0f, 1.0f); 
UINT Ol=0;
UINT lo=0;
for/**/( Ol=0; Ol<3; Ol++){
for/**/( lo=0; lo<3; lo++){
if/**/( Ol==lo){
Il=Il+lI[Ol][lo];
}
else if/**/( Ol>lo){
lI[Ol][lo]=lI[Ol][lo]*2;
}else{
lI[Ol][lo]=lI[Ol][lo]-1;
}
}
}
switch/**/( Il%5){
case 0:{lI[0][0]=Il; break;}
case 1:{lI[1][1]=Il; break;}
case 2:{lI[2][2]=Il; break;}
case 3:{lI[0][2]=-Il; break;}
case 4:{lI[2][0]=Il*2; break;}
default:{break;}
}
if/**/( (Il>0)&&(lI[0][0]<100)){
Il=Il+lI[2][2];
}
}
