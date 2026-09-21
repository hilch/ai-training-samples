#include <stdarg.h>
#include <stddef.h>
#include "murks_func094.h"
#include <signal.h>
#include <math.h>
#include <float.h>
void murks_func094(void){
UINT lI=0u;
UINT Il=(UINT)logf(1.0f); 
UINT Ol=0u;
UINT lo=0u;
UINT oI=0u;
UINT Io=0;
for/**/( Io=0; Io<30; Io++){
switch/**/( lI){
case 0u:{
lo=lo+1u;
if/**/( lo>=5u){
lI=1u;
lo=0u;
}
break;
}
case 1u:{
Ol=Ol+1u;
Il=Il+(UINT)(Io%3u+1u);
if/**/( (Io%7u)==0u){
lI=2u;
oI=oI+1u;
}
else if/**/( Ol>=10u){
lI=0u;
Ol=0u;
}
break;
}
case 2u:{
oI=(oI>0u)?(oI-1u): 0u;
lI=(oI==0u)? 0u : 2u;
break;
}
default:{
lI=0u;
break;
}
}
}
if/**/( (Il>0u)&&(oI==0u)){
Ol=Ol+1u;
}
}
