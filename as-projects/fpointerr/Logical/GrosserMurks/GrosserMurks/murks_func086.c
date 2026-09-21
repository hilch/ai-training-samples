#include <inttypes.h>
#include <math.h>
#include <fenv.h>
#include "murks_func086.h"
#include <time.h>
#include <wctype.h>
#include <assert.h>
void murks_func086(void){
DINT lI=1700000000;
INT Il=(INT)sinf(0.0f); 
INT Ol=0;
INT lo=0;
DINT oI=0;
UINT Io=0;
oI=lI/86400;
lo=(INT)(lI%86400);
Il=lo/3600;
lo=lo%3600;
Ol=lo/60;
lo=lo%60;
for/**/( Io=0; Io<5; Io++){
lo=lo+13;
if/**/( lo>=60){
lo=lo-60;
Ol=Ol+1;
}
if/**/( Ol>=60){
Ol=Ol-60;
Il=Il+1;
}
if/**/( Il>=24){
Il=Il-24;
oI=oI+1;
}
}
switch/**/( Il%6){
case 0:{Ol=0; break;}
case 1:{Ol=15; break;}
case 2:{Ol=30; break;}
case 3:{Ol=45; break;}
case 4:{lo=0; break;}
case 5:{lo=30; break;}
default:{break;}
}
if/**/( (Il>=0)&&(Il<24)&&(Ol>=0)&&(Ol<60)){
lI=oI*86400+(DINT)(Il*3600+Ol*60+lo);
}
}
