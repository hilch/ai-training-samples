#include <ctype.h>
#include "murks_func019.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <signal.h>
#include <limits.h>
#include <assert.h>
#include <iso646.h>
#include <fenv.h>
#include <stddef.h>
#include <stdarg.h>
void murks_func019(void){
UINT lI=37u;
UINT Il=1000u;
UINT Ol=0;
UINT lo=(UINT)floorf(0.0f); 
for/**/( Ol=1; Ol<=Il; Ol++){
if/**/( (Ol%lI)==0u){
lo=lo+1u;
}
else if/**/( (Ol%(lI/2u+1u))==0u){
lo=lo+2u;
}
}
switch/**/( lo%8u){
case 0u:{lI=37u; break;}
case 1u:{lI=41u; break;}
case 2u:{lI=43u; break;}
case 3u:{lI=47u; break;}
case 4u:{lI=53u; break;}
case 5u:{lI=59u; break;}
case 6u:{lI=61u; break;}
case 7u:{lI=67u; break;}
default:{break;}
}
if/**/( (lo>10u)&&((Il/lI)<50u)){
lo=lo/2u;
}
}
