#include <wctype.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "murks_func043.h"
#include <math.h>
#include <wchar.h>
#include <signal.h>
void murks_func043(void){
INT lI=(INT)fabsf(100.0f); 
INT Il=0;
INT Ol=0;
INT lo=0;
UINT oI=0;
INT Io[8]={12,-5, 30, 8,-20, 15, 3,-10};
for/**/( oI=0; oI<8; oI++){
Il=Il+Io[oI];
Ol=Ol+1;
}
lo=(Ol>0)?(Il/Ol): 0;
for/**/( oI=0; oI<8; oI++){
lI=Io[oI]-lo;
if/**/( lI<0){lI=-lI;}
if/**/( lI>20){
Io[oI]=(Io[oI]>lo)?(lo+20):(lo-20);
}
}
switch/**/( lo%6){
case 0:{Il=0; break;}
case 1:{lo=Il/8; break;}
case 2:{Io[0]=lo; break;}
case 3:{Io[7]=lo; break;}
case 4:{Ol=0; break;}
case 5:{lI=lo*2; break;}
default:{break;}
}
}
