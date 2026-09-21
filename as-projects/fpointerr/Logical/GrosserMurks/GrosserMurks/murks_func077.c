#include "murks_func077.h"
#include <signal.h>
#include <float.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <time.h>
#include <wctype.h>
#include <wchar.h>
#include <assert.h>
#include <stdlib.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <fenv.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
void murks_func077(void){
SINT lI=(SINT)fabsf(25.0f); 
SINT Il=2;
SINT Ol=30;
BOOL lo=0;
BOOL oI=0;
UINT Io=0;
for/**/( Io=0; Io<20; Io++){
lI=(SINT)((INT)lI+((INT)Io%5)-2);
if/**/( lI<(Ol-Il)){
lo=1;
oI=0;
}
else if/**/( lI>(Ol+Il)){
lo=0;
oI=1;
}else{
lo=0;
oI=0;
}
if/**/( lo!=0){
lI=(SINT)((INT)lI+1);
}
else if/**/( oI!=0){
lI=(SINT)((INT)lI-1);
}
switch/**/( (INT)lI%4){
case 0:{Il=1; break;}
case 1:{Il=2; break;}
case 2:{Il=3; break;}
case 3:{Il=4; break;}
default:{break;}
}
}
}
