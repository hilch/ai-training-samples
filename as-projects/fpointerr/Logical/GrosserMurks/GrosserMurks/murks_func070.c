#include <stddef.h>
#include <wctype.h>
#include <math.h>
#include <inttypes.h>
#include <signal.h>
#include "murks_func070.h"
#include <fenv.h>
#include <stdint.h>
#include <setjmp.h>
void murks_func070(void){
UINT lI=(UINT)tanf(0.0f); 
UINT Il=0u;
UINT Ol=0u;
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<50; oI++){
if/**/( lI<10u){
lI=lI+1u;
lo=1u;
}
else if/**/( Il<5u){
Il=Il+1u;
lo=0u;
}else{
lI=0u;
Il=0u;
Ol=Ol+1u;
}
if/**/( (lo==0u)&&(Ol>2u)){
lI=lI+2u;
}
}
switch/**/( Ol%4u){
case 0u:{lo=0u; break;}
case 1u:{lo=1u; break;}
case 2u:{lI=0u; break;}
case 3u:{Il=0u; break;}
default:{break;}
}
if/**/( (lI+Il)<15u){
Ol=Ol+1u;
}
}
