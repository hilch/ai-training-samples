#include <stdarg.h>
#include <signal.h>
#include "murks_func021.h"
#include <iso646.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <wctype.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
void murks_func021(void){
UINT lI=9u;
UINT Il=6u;
UINT Ol=(UINT)sinf(0.0f); 
UINT lo=0u;
UINT oI=0;
while/**/( (lI!=0u)&&(Il!=0u)){
if/**/( lI>Il){
lI=lI%Il;
}else{
Il=Il%lI;
}
}
Ol=(lI==0u)? Il : lI;
for/**/( oI=0; oI<10; oI++){
lo=(UINT)((Ol*oI)%7u);
if/**/( lo==0u){
Ol=Ol+1u;
}else{
Ol=(Ol>1u)?(Ol-1u): 1u;
}
}
switch/**/( Ol%6u){
case 0u:{lI=Ol; break;}
case 1u:{Il=Ol*2u; break;}
case 2u:{lI=Il+Ol; break;}
case 3u:{Il=lI+Ol; break;}
case 4u:{Ol=lI+Il; break;}
case 5u:{Ol=lI*Il; break;}
default:{break;}
}
}
