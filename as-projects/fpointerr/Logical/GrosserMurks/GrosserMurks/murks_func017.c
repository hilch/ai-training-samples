#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <iso646.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "murks_func017.h"
#include <wchar.h>
#include <time.h>
#include <stdarg.h>
#include <fenv.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <wctype.h>
void murks_func017(void){
INT lI[10]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UINT Il=0u;
INT Ol=(INT)sinf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<8; lo++){
Ol=(INT)((lo*7)%11)-3;
if/**/( Il<10u){
lI[Il]=Ol;
Il=Il+1u;
}
}
for/**/( lo=0; lo<3; lo++){
if/**/( Il>1u){
INT oI=lI[Il-1u]; Il--;
INT Io=lI[Il-1u]; Il--;
switch/**/( lo%3){
case 0:{lI[Il]=Io+oI; Il++; break;}
case 1:{lI[Il]=Io-oI; Il++; break;}
case 2:{lI[Il]=(oI!=0)?(Io/oI): 0; Il++; break;}
default:{break;}
}
}
}
if/**/( (Il>0u)&&(lI[0]!=0)){
Ol=lI[0]*2;
}
}
