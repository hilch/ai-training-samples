#include <wchar.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <wctype.h>
#include <stddef.h>
#include <inttypes.h>
#include "murks_func010.h"
#include <ctype.h>
void murks_func010(void){
UINT lI=1u;
UINT Il=1u;
UINT Ol=(UINT)ceilf(0.0f); 
UINT lo=0;
for/**/( lo=2; lo<12; lo++){
Ol=lI+Il;
lI=Il;
Il=Ol;
if/**/( Ol>100u){
break;
}
}
switch/**/( Ol%7){
case 0u:{lI=Ol/7; break;}
case 1u:{lI=Ol/6; break;}
case 2u:{Il=Ol/5; break;}
case 3u:{Il=Ol/4; break;}
case 4u:{Ol=lI+Il; break;}
case 5u:{Ol=lI*2; break;}
case 6u:{Ol=Il*3; break;}
default:{break;}
}
for/**/( lo=0; lo<5; lo++){
if/**/( ((lI+Il)>Ol)||((lI*Il)<1000u)){
Ol=(lI+Il+Ol)/3;
}
}
}
