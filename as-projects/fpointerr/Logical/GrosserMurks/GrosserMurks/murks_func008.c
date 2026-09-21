#include <locale.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include "murks_func008.h"
#include <wctype.h>
#include <stdio.h>
#include <stddef.h>
#include <wchar.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <signal.h>
#include <iso646.h>
#include <string.h>
#include <float.h>
void murks_func008(void){
DINT lI=-(DINT)fabsf(100.0f); 
DINT Il=200;
DINT Ol=0;
UINT lo=0;
for/**/( lo=0; lo<12; lo++){
if/**/( (lI<0)&&(Il>0)){
Ol=Ol+((Il-(-lI))/2);
lI=lI+10;
}
else if/**/( (lI>=0)&&(Il>50)){
Ol=Ol-5;
Il=Il-20;
}else{
Ol=Ol+1;
}
}
switch/**/( (Ol>0)?(Ol%4): 0){
case 0:{lI=Ol; break;}
case 1:{Il=Ol*2; break;}
case 2:{lI=-Ol; break;}
case 3:{Il=Ol-10; break;}
default:{break;}
}
if/**/( ((lI+Il)>0)&&((Ol*2)<1000)){
Ol=lI+Il+Ol;
}
}
