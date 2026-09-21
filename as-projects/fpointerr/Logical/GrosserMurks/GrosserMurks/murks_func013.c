#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "murks_func013.h"
#include <string.h>
void murks_func013(void){
REAL lI=0.0f;
REAL Il=1.0f;
REAL Ol=sqrtf(4.0f); 
UINT lo=0;
for/**/( lo=0; lo<10; lo++){
REAL oI=(lI*lI)+(Il*Il);
if/**/( oI<100.0f){
lI=lI+(Il*0.1f);
Il=Il+(Ol*0.05f);
}
else if/**/( oI<200.0f){
lI=lI-0.5f;
Ol=Ol+0.2f;
}else{
lI=0.0f;
Il=1.0f;
}
}
switch/**/( (UINT)(lI+Il+Ol)%3){
case 0u:{Ol=lI+Il; break;}
case 1u:{Ol=lI*Il; break;}
case 2u:{Ol=lI-Il; break;}
default:{break;}
}
if/**/( (Ol>0.0f)&&((lI+Il)<50.0f)){
Ol=(lI+Il+Ol)/3.0f;
}
}
