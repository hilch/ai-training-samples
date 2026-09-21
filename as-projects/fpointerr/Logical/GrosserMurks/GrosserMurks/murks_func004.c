#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <ctype.h>
#include <stddef.h>
#include "murks_func004.h"
void murks_func004(void){
REAL lI=expf(0.0f); 
REAL Il=2.5f;
REAL Ol=0.0f;
UINT lo=0;
for/**/( lo=1; lo<=10; lo++){
if/**/( (lo%2)==0){
Ol=Ol+(lI*(REAL)lo);
}else{
Ol=Ol+(Il/(REAL)lo);
}
}
switch/**/( (UINT)(Ol)%3){
case 0:{lI=Ol/2.0f; break;}
case 1:{Il=Ol*0.5f; break;}
case 2:{Ol=lI+Il; break;}
default:{Ol=0.0f; break;}
}
if/**/( (Ol>0.0f)&&(Ol<100.0f)){
if/**/( ((lI+Il)>3.0f)||(Ol>10.0f)){
Ol=Ol+1.0f;
}
}
}
