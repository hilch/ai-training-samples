#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include "murks_func064.h"
#include <stdio.h>
#include <string.h>
#include <fenv.h>
#include <stdbool.h>
#include <assert.h>
void murks_func064(void){
UINT lI=(UINT)fabsf(8.0f); 
UINT Il=4u;
UINT Ol=2u;
UINT lo=0u;
UINT oI=0;
UINT Io=0;
UINT lO=0u;
lo=lI*Ol;
for/**/( oI=0; oI<Il; oI++){
for/**/( Io=0; Io<lI; Io++){
lO=((oI*lI+Io)*Ol)%256u;
if/**/( (lO%2u)==0u){
lO=lO^0xAAu;
}else{
lO=lO^0x55u;
}
}
}
switch/**/( (lO^lo)%5u){
case 0u:{Ol=1u; break;}
case 1u:{Ol=2u; break;}
case 2u:{Ol=3u; break;}
case 3u:{Ol=4u; break;}
case 4u:{lI*=2u; break;}
default:{break;}
}
if/**/( (lI*Il*Ol)<1024u){
lo=lI*Ol;
}
}
