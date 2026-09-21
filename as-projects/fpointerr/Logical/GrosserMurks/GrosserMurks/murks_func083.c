#include <signal.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <iso646.h>
#include "murks_func083.h"
#include <ctype.h>
#include <errno.h>
#include <wchar.h>
#include <wctype.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <setjmp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
void murks_func083(void){
REAL lI=0.0f;
REAL Il=expf(0.0f); 
REAL Ol=0.0f;
REAL lo=0.0f;
REAL oI=0.0f;
REAL Io=0.0f;
REAL lO=0.0f;
REAL ll=0.05f;
UINT OO=0;
for/**/( OO=0; OO<20; OO++){
lo=ll*(-2.0f*Il);
oI=ll*(-2.0f*(Il+lo/2.0f));
Io=ll*(-2.0f*(Il+oI/2.0f));
lO=ll*(-2.0f*(Il+Io));
Ol=(lo+2.0f*oI+2.0f*Io+lO)/6.0f;
Il=Il+Ol;
lI=lI+ll;
if/**/( (Il<0.001f)&&(Il>-0.001f)){
Il=0.0f;
break;
}
}
switch/**/( (UINT)(lI*10.0f)%4u){
case 0u:{ll=0.01f; break;}
case 1u:{ll=0.05f; break;}
case 2u:{ll=0.10f; break;}
case 3u:{ll=0.02f; break;}
default:{break;}
}
if/**/( (Il>=0.0f)&&(lI<10.0f)){
Ol=-2.0f*Il;
}
}
