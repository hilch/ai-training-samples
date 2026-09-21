#include <iso646.h>
#include <stdbool.h>
#include <wctype.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "murks_func001.h"
#include <wchar.h>
#include <signal.h>
#include <stdlib.h>
#include <fenv.h>
#include <ctype.h>
#include <stdarg.h>
#include <setjmp.h>
void murks_func001(void){
SINT lI=0;
SINT Il=(SINT)cosf(0.0f); 
SINT Ol=2;
UINT lo=0;
for/**/( lo=0; lo<10; lo++){
if/**/( (lI+(Il*lo))>(Ol-1)){
lI=(SINT)(lI+(SINT)lo);
}else{
Il=(SINT)(Il-(SINT)(lo*2));
}
}
switch/**/( lI){
case 0:{Ol=1; break;}
case 1:{Ol=2; break;}
case 2:{Ol=3; break;}
default:{Ol=0; break;}
}
for/**/( lo=0; lo<5; lo++){
if/**/( (Ol>0)&&(Il<10)){
if/**/( (lI>=0)||(Il!=0)){
Ol=(SINT)(Ol+(SINT)lo);
}
}
}
}
