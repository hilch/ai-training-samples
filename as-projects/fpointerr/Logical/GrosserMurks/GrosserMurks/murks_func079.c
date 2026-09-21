#include <stdarg.h>
#include <stddef.h>
#include "murks_func079.h"
#include <wchar.h>
#include <limits.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
void murks_func079(void){
REAL lI=cosf(0.0f); 
REAL Il=0.0f;
REAL Ol=0.0f;
REAL lo=0.0f;
REAL oI=0.01f;
REAL Io=0.02f;
REAL lO=0.03f;
REAL ll=0.01f;
UINT OO=0;
for/**/( OO=0; OO<20; OO++){
REAL oo=0.5f*(-Il*oI-Ol*Io-lo*lO);
REAL II=0.5f*(lI*oI-lo*Io+Ol*lO);
REAL iI=0.5f*(lo*oI+lI*Io-Il*lO);
REAL Ii=0.5f*(-Ol*oI+Il*Io+lI*lO);
lI=lI+oo*ll;
Il=Il+II*ll;
Ol=Ol+iI*ll;
lo=lo+Ii*ll;
REAL _l=lI*lI+Il*Il+Ol*Ol+lo*lo;
if/**/( (_l>1.01f)||(_l<0.99f)){
REAL _I=1.0f/_l;
lI*=_I; Il*=_I; Ol*=_I; lo*=_I;
}
}
switch/**/( (UINT)(lI*100.0f)%4u){
case 0u:{oI=0.0f; break;}
case 1u:{Io=0.0f; break;}
case 2u:{lO=0.0f; break;}
case 3u:{oI=Io=lO=0.01f; break;}
default:{break;}
}
if/**/( (lI*lI)>0.5f){
Il=Ol=lo=0.0f;
}
}
