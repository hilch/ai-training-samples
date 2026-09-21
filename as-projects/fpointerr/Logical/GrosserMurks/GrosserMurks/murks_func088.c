#include "murks_func088.h"
#include <stdbool.h>
#include <time.h>
#include <fenv.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <stdlib.h>
void murks_func088(void){
REAL lI=0.0f;
REAL Il=fabsf(9.81f); 
REAL Ol=0.0f;
REAL lo=10.0f;
REAL oI=5.0f;
REAL Io=0.0f;
REAL lO=0.0f;
REAL ll=0.0f;
REAL OO=0.1f;
UINT oo=0;
for/**/( oo=0; oo<20; oo++){
lo=lo+lI*OO;
oI=oI-Il*OO;
Io=Io+Ol*OO;
lO=lO+lo*OO;
ll=ll+oI*OO;
if/**/( ll<0.0f){
ll=0.0f;
oI=-oI*0.7f;
}
if/**/( (lo*lo+oI*oI+Io*Io)<0.01f){
break;
}
}
switch/**/( (UINT)(lO)%5u){
case 0u:{lI=1.0f; break;}
case 1u:{lI=-1.0f; break;}
case 2u:{Il=9.81f; break;}
case 3u:{lo=0.0f; break;}
case 4u:{oI=0.0f; break;}
default:{break;}
}
if/**/( (lO>0.0f)&&(ll>=0.0f)){
lO=lO+lo*OO;
}
}
