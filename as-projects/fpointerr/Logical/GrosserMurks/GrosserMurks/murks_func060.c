#include <wchar.h>
#include <inttypes.h>
#include <math.h>
#include <locale.h>
#include <time.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <iso646.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <fenv.h>
#include <wctype.h>
#include "murks_func060.h"
void murks_func060(void){
INT lI=(INT)cosf(0.0f); 
INT Il=0;
INT Ol[5]={-10, 0, 5, 20,-3};
INT lo[5]={0, 0, 0, 0, 0};
UINT oI=0;
for/**/( oI=0; oI<5; oI++){
lo[oI]=(lI*Ol[oI])+Il;
if/**/( lo[oI]>15){lo[oI]=15;}
if/**/( lo[oI]<-15){lo[oI]=-15;}
switch/**/( lo[oI]%4){
case 0:{lI=lI+1; break;}
case 1:{lI=lI-1; break;}
case-1:{Il=Il+1; break;}
case 3:{Il=Il-1; break;}
default:{break;}
}
if/**/( (lI<-5)||(lI>5)){
lI=1;
}
if/**/( (Il<-10)||(Il>10)){
Il=0;
}
}
}
