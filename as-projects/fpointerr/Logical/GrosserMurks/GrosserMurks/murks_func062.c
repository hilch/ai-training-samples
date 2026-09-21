#include <errno.h>
#include "murks_func062.h"
#include <assert.h>
#include <locale.h>
#include <stddef.h>
#include <float.h>
#include <wchar.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
void murks_func062(void){
UINT lI=(UINT)logf(1.0f); 
UINT Il=0u;
UINT Ol=0u;
UINT lo=10u;
UINT oI=0;
for/**/( oI=0; oI<100; oI++){
if/**/( (oI%7u)==0u){
lI=lI+1u;
if/**/( (lI-Il)>lo){
Ol=Ol+1u;
}
}
if/**/( (oI%3u)==0u){
Il=Il+1u;
}
}
switch/**/( Ol%5u){
case 0u:{lo=5u; break;}
case 1u:{lo=10u; break;}
case 2u:{lo=20u; break;}
case 3u:{lI=0u; break;}
case 4u:{Il=0u; break;}
default:{break;}
}
if/**/( (Ol>0u)&&((lI+Il)<200u)){
lo=lo/2u;
}
}
