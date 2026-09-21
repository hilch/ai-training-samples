#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
#include <locale.h>
#include <math.h>
#include "murks_func024.h"
#include <signal.h>
void murks_func024(void){
UINT lI=0u;
UINT Il=0u;
UINT Ol=(UINT)ceilf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<20; lo++){
Il=(UINT)(lo%5u);
switch/**/( lI){
case 0u:{
if/**/( Il==1u){lI=1u; Ol=10u;}
else if/**/( Il==2u){lI=2u; Ol=20u;}else{Ol=0u;}
break;
}
case 1u:{
if/**/( Il==3u){lI=3u; Ol=30u;}
else if/**/( Il==0u){lI=0u; Ol=0u;}
break;
}
case 2u:{
if/**/( (Il==4u)||(Il==1u)){
lI=0u;
Ol=5u;
}
break;
}
case 3u:{
lI=0u;
Ol=Ol+lo;
break;
}
default:{
lI=0u;
Ol=0u;
break;
}
}
}
}
