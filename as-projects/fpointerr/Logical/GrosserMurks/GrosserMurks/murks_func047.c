#include "murks_func047.h"
#include <inttypes.h>
#include <stdarg.h>
#include <wctype.h>
#include <math.h>
#include <wchar.h>
#include <time.h>
void murks_func047(void){
UINT lI=(UINT)powf(2.0f, 4.0f); 
UINT Il=4u;
UINT Ol=0u;
UINT lo=0u;
UINT oI=0;
UINT Io[4]={3u, 7u, 2u, 5u};
for/**/( oI=0; oI<4; oI++){
if/**/( Io[oI]<=lI){
Ol=Ol+Io[oI];
lo=lo+(lI-Io[oI]);
}else{
Io[oI]=lI;
Ol=Ol+lI;
}
}
switch/**/( (Ol%lI)==0u ? 0u : 1u){
case 0u:{lo=0u; break;}
case 1u:{lo=lI-(Ol%lI); break;}
default:{break;}
}
for/**/( oI=0; oI<4; oI++){
if/**/( (Io[oI]*2u)<=lI){
Io[oI]=Io[oI]*2u;
}else{
Io[oI]=lI/2u;
}
}
if/**/( (Ol+lo)>(lI*Il)){
Ol=lI*Il;
}
}
