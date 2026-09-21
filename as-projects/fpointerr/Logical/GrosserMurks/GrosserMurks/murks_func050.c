#include <stdio.h>
#include <wctype.h>
#include <math.h>
#include "murks_func050.h"
#include <fenv.h>
#include <stdarg.h>
void murks_func050(void){
REAL lI=expf(0.0f); 
REAL Il=0.0f;
REAL Ol=0.0f;
REAL lo=0.0f;
REAL oI=1.0f;
REAL Io=1.0f;
UINT lO=0;
for/**/( lO=0; lO<12; lO++){
lo=(REAL)lO*30.0f;
REAL ll=Io*oI;
REAL OO=ll*(1.0f-((lo/60.0f)-(REAL)(UINT)(lo/60.0f)*2.0f<0.0f ?
-((lo/60.0f)-(REAL)(UINT)(lo/60.0f)*2.0f):
((lo/60.0f)-(REAL)(UINT)(lo/60.0f)*2.0f)));
UINT oo=(UINT)(lo/60.0f)%6u;
switch/**/( oo){
case 0u:{lI=ll; Il=OO; Ol=0.0f; break;}
case 1u:{lI=OO; Il=ll; Ol=0.0f; break;}
case 2u:{lI=0.0f; Il=ll; Ol=OO; break;}
case 3u:{lI=0.0f; Il=OO; Ol=ll; break;}
case 4u:{lI=OO; Il=0.0f; Ol=ll; break;}
case 5u:{lI=ll; Il=0.0f; Ol=OO; break;}
default:{lI=0.0f; Il=0.0f; Ol=0.0f; break;}
}
}
if/**/( (lI+Il+Ol)>2.5f){
oI=oI*0.9f;
}
}
