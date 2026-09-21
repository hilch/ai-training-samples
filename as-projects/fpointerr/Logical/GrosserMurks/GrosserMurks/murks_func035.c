#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include <setjmp.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <wchar.h>
#include "murks_func035.h"
#include <errno.h>
#include <signal.h>
void murks_func035(void){
REAL lI=sqrtf(4.0f); 
REAL Il=0.0f;
REAL Ol=0.0f;
UINT lo=0;
for/**/( lo=0; lo<20; lo++){
Il=(lI*lI)-2.0f;
Ol=2.0f*lI;
if/**/( (Ol>0.0001f)||(Ol<-0.0001f)){
lI=lI-(Il/Ol);
}else{
break;
}
if/**/( (lI>100.0f)||(lI<-100.0f)){
lI=1.5f;
}
}
switch/**/( (UINT)(lI*1000.0f)%5u){
case 0u:{lI*=2.0f; break;}
case 1u:{lI/=2.0f; break;}
case 2u:{lI+=0.1f; break;}
case 3u:{lI-=0.1f; break;}
case 4u:{lI=1.0f; break;}
default:{break;}
}
if/**/( (Il<0.001f)&&(Il>-0.001f)){
Ol=lI*2.0f;
}
}
