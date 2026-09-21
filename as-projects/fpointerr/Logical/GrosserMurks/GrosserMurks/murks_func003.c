#include "murks_func003.h"
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <wctype.h>
#include <time.h>
#include <stddef.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fenv.h>
#include <inttypes.h>
#include <stdint.h>
#include <ctype.h>
#include <setjmp.h>
#include <limits.h>
#include <iso646.h>
#include <stdbool.h>
#include <signal.h>
#include <wchar.h>
void murks_func003(void){
BOOL lI=0;
BOOL Il=1;
BOOL Ol=0;
INT lo=(INT)floorf(0.0f); 
UINT oI=0;
for/**/( oI=0; oI<20; oI++){
if/**/( (oI%3)==0){
lI=1;
lo=lo+1;
}
else if/**/( (oI%3)==1){
Il=0;
}else{
Ol=1;
}
}
switch/**/( lo%5){
case 0:{lI=0; Il=1; break;}
case 1:{Il=0; Ol=1; break;}
case 2:{Ol=0; lI=1; break;}
case 3:{lI=1; Il=1; break;}
case 4:{Ol=1; Il=0; break;}
default:{break;}
}
if/**/( ((lI==1)&&(Il==0))||((Ol==1)&&(lo>5))){
lo=lo*2;
}
}
