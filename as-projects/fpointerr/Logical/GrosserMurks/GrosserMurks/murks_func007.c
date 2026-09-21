#include "murks_func007.h"
#include <stdlib.h>
#include <wchar.h>
#include <stdarg.h>
#include <locale.h>
#include <fenv.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include <float.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <iso646.h>
void murks_func007(void){
UINT lI=0u;
UINT Il=0u;
UINT Ol=(UINT)tanf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<15; lo++){
switch/**/( lI){
case 0u:{
if/**/( (lo%2)==0){
Il=1u;
}else{
Il=2u;
}
Ol=Ol+1;
break;
}
case 1u:{
if/**/( (lo>5)&&(Ol<10)){
Il=3u;
}else{
Il=0u;
}
break;
}
case 2u:{
Il=(Ol>7)? 0u : 1u;
break;
}
case 3u:{
Il=0u;
Ol=0;
break;
}
default:{
Il=0u;
break;
}
}
lI=Il;
}
}
