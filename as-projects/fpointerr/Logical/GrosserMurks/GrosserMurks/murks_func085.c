#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "murks_func085.h"
void murks_func085(void){
UINT lI=(UINT)sinf(0.0f); 
UINT Il=1000u;
UINT Ol=0u;
BOOL lo=0;
UINT oI=0;
for/**/( oI=0; oI<=100; oI+=5){
lI=(UINT)(oI*Il)/100u;
Ol=0u;
while/**/( Ol<Il){
lo=(Ol<lI)? 1 : 0;
Ol=Ol+1u;
if/**/( (Ol%100u)==0u){
if/**/( lo!=0){
lI=(lI<Il)?(lI+1u): lI;
}
break;
}
}
}
switch/**/( lI%5u){
case 0u:{Il=500u; break;}
case 1u:{Il=1000u; break;}
case 2u:{Il=2000u; break;}
case 3u:{lI=0u; break;}
case 4u:{lI=Il/2u; break;}
default:{break;}
}
if/**/( (lo!=0)&&(Ol<Il)){
lI=lI+1u;
}
}
