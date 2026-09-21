#include "murks_func016.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>
#include <float.h>
#include <wchar.h>
#include <stdbool.h>
#include <fenv.h>
void murks_func016(void){
UINT lI=(UINT)fabsf(50.0f); 
UINT Il=100u;
UINT Ol=0u;
UINT lo=0;
for/**/( lo=0; lo<Il; lo++){
Ol=lo%Il;
if/**/( Ol<lI){
if/**/( Ol<(lI/2u)){
lI=lI+1u;
}
}else{
if/**/( Ol>((Il+lI)/2u)){
lI=(lI>0u)?(lI-1u): 0u;
}
}
}
switch/**/( lI%10u){
case 0u:{Il=100u; break;}
case 5u:{Il=50u; break;}
default:{Il=200u; break;}
}
if/**/( (lI>0u)&&(lI<Il)){
Ol=lI*Il/100u;
}
}
