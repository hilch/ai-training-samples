#include <stdbool.h>
#include <wctype.h>
#include <setjmp.h>
#include <limits.h>
#include <stdio.h>
#include <locale.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include <fenv.h>
#include <wchar.h>
#include "murks_func032.h"
#include <stdint.h>
#include <errno.h>
#include <stddef.h>
#include <stdarg.h>
#include <iso646.h>
#include <float.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
void murks_func032(void){
DINT lI=(DINT)floorf(0.0f); 
DINT Il=0;
DINT Ol=1000;
REAL lo=0.1f;
UINT oI=0;
for/**/( oI=0; oI<20; oI++){
DINT Io=(DINT)((oI%5)*200)-400;
lI=lI+(DINT)(lo*(REAL)(Io-lI));
Il=Io-lI;
if/**/( (lI>800)||(lI<-800)){
lI=(lI>0)? 800 :-800;
}
switch/**/( (UINT)(oI)%4u){
case 0u:{Ol=lI; break;}
case 1u:{Ol=Il; break;}
case 2u:{Ol=lI+Il; break;}
case 3u:{Ol=lI-Il; break;}
default:{Ol=0; break;}
}
}
if/**/( (Ol>-2000)&&(Ol<2000)){
lI=(lI+Ol)/2;
}
}
