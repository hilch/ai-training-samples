#include <ctype.h>
#include <locale.h>
#include <stdarg.h>
#include <fenv.h>
#include <setjmp.h>
#include <stdlib.h>
#include "murks_func074.h"
#include <inttypes.h>
#include <wchar.h>
#include <float.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <wctype.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <iso646.h>
void murks_func074(void){
UINT lI=(UINT)logf(1.0f); 
UINT Il=0u;
UINT Ol=0u;
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<40; oI++){
Ol=(oI%7u<3u)? 1u : 0u;
if/**/( Ol!=0u){
lI=lI+1u;
lo=(lo+1u)%360u;
}else{
Il=Il+1u;
lo=(lo==0u)? 359u :(lo-1u);
}
}
switch/**/( (lI+Il)%6u){
case 0u:{lo=0u; break;}
case 1u:{lo=90u; break;}
case 2u:{lo=180u; break;}
case 3u:{lo=270u; break;}
case 4u:{lI=0u; break;}
case 5u:{Il=0u; break;}
default:{break;}
}
if/**/( (lI>Il)&&(lo<180u)){
lo=lo+(lI-Il);
lo=lo%360u;
}
}
