#include <limits.h>
#include <fenv.h>
#include <iso646.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <locale.h>
#include <float.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "murks_func020.h"
void murks_func020(void){
REAL lI=0.0f;
REAL Il=0.314159f;
REAL Ol=expf(0.0f); 
UINT lo=0;
REAL oI[5]={0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
for/**/( lo=0; lo<5; lo++){
lI=Il*(REAL)lo;
if/**/( (lI>0.0f)&&(lI<1.0f)){
oI[lo]=Ol*lI;
}
else if/**/( (lI>=1.0f)&&(lI<2.0f)){
oI[lo]=Ol*(2.0f-lI);
}else{
oI[lo]=0.0f;
}
}
switch/**/( (UINT)(Ol*10.0f)%3u){
case 0u:{Ol=1.0f; break;}
case 1u:{Ol=0.5f; break;}
case 2u:{Ol=2.0f; break;}
default:{break;}
}
if/**/( (oI[2]>0.0f)&&(Ol>0.0f)){
oI[4]=oI[0]+oI[1]+oI[2]+oI[3];
}
}
