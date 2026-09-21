#include <float.h>
#include "murks_func091.h"
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <wctype.h>
#include <inttypes.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
void murks_func091(void){
UINT lI=(UINT)cosf(0.0f); 
UINT Il=0u;
UINT Ol=100u;
UINT lo=50u;
UINT oI=10u;
UINT Io=5u;
UINT lO=0;
for/**/( lO=0; lO<10; lO++){
Il=Il+(UINT)((lO+1u)*15u);
if/**/( Il>=(lI*100u)){
lI=lI+1u;
Ol=Ol+10u;
lo=lo+5u;
oI=oI+2u;
Io=Io+1u;
Il=Il-(lI-1u)*100u;
}
switch/**/( lI%4u){
case 0u:{oI=oI+5u; break;}
case 1u:{Io=Io+3u; break;}
case 2u:{Ol=Ol+20u; break;}
case 3u:{lo=lo+10u; break;}
default:{break;}
}
if/**/( (Ol+lo)>500u){
Ol=100u;
lo=50u;
}
}
}
