#include <locale.h>
#include <math.h>
#include "murks_func054.h"
#include <limits.h>
#include <errno.h>
#include <wchar.h>
#include <iso646.h>
#include <signal.h>
#include <wctype.h>
#include <string.h>
#include <stdio.h>
void murks_func054(void){
REAL lI=1.0f;
REAL Il=0.5f;
REAL Ol=0.2f;
REAL lo=fabsf(50.0f); 
REAL oI=20.0f;
REAL Io=0.0f;
REAL lO=0.0f;
REAL ll=0.0f;
UINT OO=0;
for/**/( OO=0; OO<25; OO++){
REAL oo=lo-oI;
lO=lO+(Il*oo*0.01f);
if/**/( lO>100.0f){lO=100.0f;}
if/**/( lO<-100.0f){lO=-100.0f;}
Io=(lI*oo)+lO+(Ol*(oo-ll)/0.01f);
ll=oo;
oI=oI+Io*0.005f;
if/**/( oI>100.0f){oI=100.0f;}
if/**/( oI<0.0f){oI=0.0f;}
}
switch/**/( (UINT)(oI)%5u){
case 0u:{lI=0.5f; break;}
case 1u:{Il=0.2f; break;}
case 2u:{Ol=0.1f; break;}
case 3u:{lo=80.0f; break;}
case 4u:{lo=30.0f; break;}
default:{break;}
}
if/**/( (oI>0.0f)&&(Io<50.0f)){
Io=Io+1.0f;
}
}
