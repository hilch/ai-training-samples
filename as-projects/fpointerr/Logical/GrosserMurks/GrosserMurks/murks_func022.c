#include <time.h>
#include <wctype.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <errno.h>
#include <ctype.h>
#include "murks_func022.h"
#include <stdlib.h>
#include <float.h>
void murks_func022(void){
SINT lI=(SINT)logf(1.0f); 
SINT Il=0;
SINT Ol=0;
UINT lo=0;
for/**/( lo=0; lo<20; lo++){
lI=(SINT)((lo*3)%50);
Il=(SINT)((lo*7)%100);
Ol=(SINT)((lo*13)%120);
if/**/( (lI>20)&&(Il>60)){
Ol=(SINT)(Ol+5);
}
else if/**/( (lI<10)&&(Il<30)){
Ol=(SINT)(Ol-3);
}
switch/**/( Ol%4){
case 0:{lI=(SINT)(lI+1); break;}
case 1:{Il=(SINT)(Il-1); break;}
case 2:{Ol=(SINT)(Ol/2); break;}
case 3:{lI=(SINT)(-lI); break;}
default:{break;}
}
}
}
