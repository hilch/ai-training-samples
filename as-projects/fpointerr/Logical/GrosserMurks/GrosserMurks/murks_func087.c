#include <stdbool.h>
#include <wchar.h>
#include "murks_func087.h"
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <locale.h>
#include <stdint.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <iso646.h>
void murks_func087(void){
INT lI[8]={0, 0, 0, 0, 0, 0, 0, 0};
INT Il[4]={7,-3, 12,-8};
INT Ol=(INT)sqrtf(4.0f); 
INT lo=-5;
UINT oI=0;
UINT Io=0;
for/**/( oI=0; oI<4; oI++){
for/**/( Io=0; Io<8; Io++){
INT lO=(Ol*Il[oI])+lo;
if/**/( lO>100){lO=100;}
if/**/( lO<-100){lO=-100;}
lI[Io]=lI[Io]+lO;
}
}
for/**/( Io=0; Io<8; Io++){
lI[Io]=lI[Io]/4;
}
switch/**/( lI[3]%7){
case 0:{Ol=1; break;}
case 1:{Ol=2; break;}
case 2:{Ol=3; break;}
case 3:{lo=0; break;}
case 4:{lo=-5; break;}
case 5:{lo=10; break;}
case 6:{Ol=-1; break;}
default:{break;}
}
if/**/( (Ol!=0)&&(lI[0]!=0)){
lI[7]=lI[0]*Ol+lo;
}
}
