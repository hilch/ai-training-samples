#include <fenv.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include "murks_func081.h"
#include <iso646.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>
#include <stddef.h>
#include <wctype.h>
void murks_func081(void){
INT lI=(INT)fabsf(10.0f); 
INT Il=10;
INT Ol=5;
INT lo=8;
INT oI=0;
INT Io=0;
UINT lO=0;
for/**/( lO=0; lO<20; lO++){
oI=(Ol>lI)? 1 :((Ol<lI)?-1 : 0);
Io=(lo>Il)? 1 :((lo<Il)?-1 : 0);
if/**/( (oI==0)&&(Io==0)){
break;
}
lI=lI+oI;
Il=Il+Io;
switch/**/( (INT)lO%5){
case 0:{Ol=Ol+1; break;}
case 1:{lo=lo-1; break;}
case 2:{Ol=Ol-1; break;}
case 3:{lo=lo+1; break;}
case 4:{Ol=lo; lo=Ol; break;}
default:{break;}
}
if/**/( (Ol<0)||(Ol>20)){Ol=10;}
if/**/( (lo<0)||(lo>20)){lo=10;}
}
}
