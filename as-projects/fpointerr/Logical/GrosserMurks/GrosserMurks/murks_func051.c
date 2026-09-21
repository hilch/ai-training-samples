#include <stddef.h>
#include <setjmp.h>
#include <math.h>
#include <signal.h>
#include "murks_func051.h"
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <wctype.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include <fenv.h>
void murks_func051(void){
UINT lI=(UINT)tanf(0.0f); 
UINT Il=1000u;
UINT Ol=0u;
BOOL lo=0;
UINT oI=0;
for/**/( oI=0; oI<200; oI++){
lI=lI+10u;
if/**/( lI<(Il/4u)){
Ol=(lI*100u)/(Il/4u);
}
else if/**/( lI<(Il*3u/4u)){
Ol=100u;
}
else if/**/( lI<Il){
Ol=100u-((lI-Il*3u/4u)*100u)/(Il/4u);
}else{
Ol=0u;
lo=1;
break;
}
}
switch/**/( Ol%5u){
case 0u:{Il=500u; break;}
case 1u:{Il=1000u; break;}
case 2u:{Il=2000u; break;}
case 3u:{Ol=50u; break;}
case 4u:{lo=0; break;}
default:{break;}
}
if/**/( (lo==1)&&(Ol==0u)){
lI=0u;
}
}
