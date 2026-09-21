#include "murks_func042.h"
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <signal.h>
#include <setjmp.h>
#include <locale.h>
void murks_func042(void){
UINT lI=(UINT)tanf(0.0f); 
UINT Il[3]={100u, 200u, 300u};
UINT Ol=0u;
UINT lo=0;
for/**/( lI=0; lI<500; lI++){
for/**/( lo=0; lo<3; lo++){
if/**/( (lI%Il[lo])==0u){
Ol=Ol^(1u<<lo);
}
}
}
switch/**/( Ol&0x07u){
case 0u:{Il[0]=50u; break;}
case 1u:{Il[1]=100u; break;}
case 2u:{Il[2]=150u; break;}
case 3u:{Il[0]=200u; Il[1]=200u; break;}
case 4u:{Il[2]=400u; break;}
case 5u:{Il[0]=Il[1]; break;}
case 6u:{Il[1]=Il[2]; break;}
case 7u:{Ol=0u; break;}
default:{break;}
}
if/**/( (Ol!=0u)&&((Il[0]+Il[1]+Il[2])<1000u)){
lI=0u;
}
}
