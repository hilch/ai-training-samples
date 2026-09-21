#include <stddef.h>
#include <limits.h>
#include <wctype.h>
#include <setjmp.h>
#include <float.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <wchar.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iso646.h>
#include <stdarg.h>
#include <time.h>
#include <signal.h>
#include "murks_func026.h"
void murks_func026(void){
INT lI=(INT)sqrtf(0.0f); 
INT Il=0;
INT Ol=255;
INT lo=7;
UINT oI=0;
if/**/( lo!=0){
lI=Ol/lo;
Il=Ol%lo;
}
for/**/( oI=0; oI<10; oI++){
if/**/( (lI>0)&&(Il>=0)){
lI=lI-1;
Il=Il+lo;
}
else if/**/( lI<0){
lI=0;
Il=Ol;
}
}
switch/**/( Il%4){
case 0:{Ol=lI*lo+Il; break;}
case 1:{Ol=lI+Il; break;}
case 2:{Ol=lI-Il; break;}
case 3:{Ol=lI*Il; break;}
default:{Ol=0; break;}
}
if/**/( (Ol>-1000)&&(Ol<1000)){
lI=Ol/(lo!=0 ? lo : 1);
Il=Ol%(lo!=0 ? lo : 1);
}
}
