#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include "murks_func030.h"
#include <stdbool.h>
#include <setjmp.h>
#include <limits.h>
#include <ctype.h>
void murks_func030(void){
SINT lI=(SINT)fabsf(5.0f); 
SINT Il=12;
SINT Ol=0;
SINT lo=0;
UINT oI=0;
lo=(lI*lI)+(Il*Il);
for/**/( oI=1; oI<=20; oI++){
if/**/( ((SINT)(oI*oI))<=lo){
Ol=(SINT)oI;
}else{
break;
}
}
switch/**/( Ol%5){
case 0:{lI=Ol; break;}
case 1:{Il=Ol; break;}
case 2:{lI=(Ol/2); break;}
case 3:{Il=(Ol-lI); break;}
case 4:{lI=Il; Il=Ol; break;}
default:{break;}
}
for/**/( oI=0; oI<5; oI++){
if/**/( ((lI+Il)>Ol)&&(((lI*Il)%3)==0)){
Ol=(SINT)(lI+Il);
}
}
}
