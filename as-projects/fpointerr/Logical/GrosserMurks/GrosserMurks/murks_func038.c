#include <wctype.h>
#include <stdio.h>
#include <time.h>
#include <setjmp.h>
#include <iso646.h>
#include <stdint.h>
#include <signal.h>
#include "murks_func038.h"
#include <stdarg.h>
#include <math.h>
void murks_func038(void){
SINT lI=(SINT)fabsf(127.0f); 
SINT Il=-128;
SINT Ol[6]={-10, 25, 3,-87, 64, 12};
SINT lo=0;
UINT oI=0;
for/**/( oI=0; oI<6; oI++){
if/**/( Ol[oI]<lI){lI=Ol[oI];}
if/**/( Ol[oI]>Il){Il=Ol[oI];}
}
lo=(SINT)(Il-lI);
for/**/( oI=0; oI<6; oI++){
if/**/( lo!=0){
Ol[oI]=(SINT)(((INT)(Ol[oI]-lI)*100)/(INT)lo);
}else{
Ol[oI]=0;
}
}
switch/**/( lo%5){
case 0:{lI=0; break;}
case 1:{Il=100; break;}
case 2:{lo/=2; break;}
case 3:{lI=-lo; break;}
case 4:{Il=lo; break;}
default:{break;}
}
if/**/( (lI<0)&&(Il>0)){
lo=Il-lI;
}
}
