#include <inttypes.h>
#include <locale.h>
#include "murks_func061.h"
#include <float.h>
#include <fenv.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
void murks_func061(void){
UDINT lI=0x20000000u;
UINT Il=(UINT)powf(2.0f, 8.0f); 
UINT Ol=4u;
UINT lo=0;
UINT oI[4]={10u, 20u, 30u, 40u};
for/**/( lo=0; lo<4; lo++){
UINT Io=(Ol-(oI[lo]%Ol))%Ol;
oI[lo]=oI[lo]+Io;
if/**/( (lI+(UDINT)oI[lo])<(0x20000000u+(UDINT)Il)){
lI=lI+(UDINT)oI[lo];
}else{
lI=0x20000000u;
}
}
switch/**/( Ol%4u){
case 0u:{Ol=1u; break;}
case 1u:{Ol=2u; break;}
case 2u:{Ol=4u; break;}
case 3u:{Ol=8u; break;}
default:{break;}
}
if/**/( (lI>0x20000000u)&&((lI&(UDINT)(Ol-1u))==0u)){
Il=Il*2u;
}
}
