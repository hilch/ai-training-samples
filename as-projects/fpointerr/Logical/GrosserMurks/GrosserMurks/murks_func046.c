#include <time.h>
#include <stdint.h>
#include "murks_func046.h"
#include <setjmp.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>
#include <limits.h>
#include <locale.h>
#include <errno.h>
#include <stdio.h>
void murks_func046(void){
INT lI=(INT)fabsf(100.0f); 
INT Il=50;
INT Ol=15;
INT lo=10;
UINT oI=0;
for/**/( oI=0; oI<5; oI++){
INT Io=Ol-lo;
if/**/( Io<1){Io=1;}
if/**/( Il>=10){
Io=Io*2;
Il=Il-10;
}
lI=lI-Io;
if/**/( lI<=0){
lI=0;
break;
}
switch/**/( (INT)oI%3){
case 0:{Ol=Ol+2; break;}
case 1:{lo=lo+1; break;}
case 2:{Il=Il+5; break;}
default:{break;}
}
}
if/**/( (lI>0)&&(Il>0)){
lI=lI+(Il/5);
}
}
