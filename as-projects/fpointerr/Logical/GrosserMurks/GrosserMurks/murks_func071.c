#include "murks_func071.h"
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <assert.h>
#include <stdint.h>
#include <float.h>
#include <iso646.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <wchar.h>
#include <errno.h>
void murks_func071(void){
INT lI=(INT)fabsf(3.0f); 
INT Il=7;
INT Ol=11;
INT lo=100;
INT oI=0;
UINT Io=0;
for/**/( Io=0; Io<20; Io++){
oI=(lI*(INT)Io*(INT)Io+Il*(INT)Io+Ol)%lo;
if/**/( oI<0){oI=oI+lo;}
switch/**/( oI%6){
case 0:{lI=lI+1; break;}
case 1:{Il=Il-1; break;}
case 2:{Ol=Ol+2; break;}
case 3:{lI=lI*2; break;}
case 4:{Il=Il/2; break;}
case 5:{Ol=Ol-3; break;}
default:{break;}
}
if/**/( (lI>50)||(lI<-50)){
lI=3;
}
if/**/( (Il>50)||(Il<-50)){
Il=7;
}
}
}
