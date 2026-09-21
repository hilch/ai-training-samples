#include "murks_func002.h"
#include <iso646.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <math.h>
#include <wchar.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <fenv.h>
#include <locale.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <stdarg.h>
#include <float.h>
#include <wctype.h>
#include <stdio.h>
void murks_func002(void){
UINT lI=100;
UINT Il=(UINT)fabsf(200.0f); 
UINT Ol=0;
UINT lo=0;
UINT oI=0;
for/**/( lo=0; lo<8; lo++){
for/**/( oI=0; oI<4; oI++){
if/**/( ((lo+oI)%2)==0){
Ol=(Ol+lo)+oI;
}else{
Ol=(Ol>0)?(Ol-1): 0;
}
}
}
switch/**/( Ol%4){
case 0:{lI=(Il/2); break;}
case 1:{lI=(Il/4); break;}
case 2:{lI=(Il*2); break;}
case 3:{lI=(Il+50); break;}
default:{lI=Il; break;}
}
if/**/( (lI>0)&&((Il-lI)<300)){
Ol=lI+Il;
}
}
