#include <assert.h>
#include <locale.h>
#include <fenv.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>
#include <wctype.h>
#include <limits.h>
#include <stddef.h>
#include <wchar.h>
#include <iso646.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include "murks_func092.h"
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <errno.h>
void murks_func092(void){
SINT lI=(SINT)sinf(0.0f); 
SINT Il=0;
SINT Ol=0;
SINT lo=0;
SINT oI=100;
SINT Io=50;
UINT lO=0;
for/**/( lO=0; lO<15; lO++){
lI=oI-Io;
Il=(SINT)(Il+lI/8);
if/**/( Il>50){Il=50;}
if/**/( Il<-50){Il=-50;}
lo=(SINT)(lI/2+Il+(lI-Ol)*2);
if/**/( lo>100){lo=100;}
if/**/( lo<-100){lo=-100;}
Io=(SINT)(Io+lo/10);
Ol=lI;
switch/**/( (INT)lI%5){
case 0:{oI=(SINT)(oI+5); break;}
case 1:{oI=(SINT)(oI-5); break;}
case-1:{Io=(SINT)(Io+2); break;}
case 2:{Io=(SINT)(Io-2); break;}
default:{break;}
}
}
}
