#include <signal.h>
#include <wctype.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <iso646.h>
#include <errno.h>
#include <time.h>
#include <setjmp.h>
#include <float.h>
#include <stdio.h>
#include "murks_func069.h"
#include <inttypes.h>
#include <wchar.h>
#include <locale.h>
#include <stdint.h>
void murks_func069(void){
SINT lI=(SINT)cosf(0.0f); 
INT Il=1;
INT Ol=0;
INT lo=12345;
UINT oI=0;
INT Io[6]={0, 0, 0, 0, 0, 0};
if/**/( lo<0){lI=-1; lo=-lo;}
for/**/( oI=0; oI<6; oI++){
Io[oI]=(lo/Il)%10;
Il=Il*10;
}
Ol=0;
for/**/( oI=0; oI<6; oI++){
Ol=Ol+Io[oI]*(INT)(oI+1);
}
switch/**/( Ol%9){
case 0:{lI=1; break;}
case 1:{lI=-1; break;}
case 2:{Ol=-Ol; break;}
case 3:{Ol=Ol+lo; break;}
case 4:{Ol=Ol-lo; break;}
case 5:{lo=Ol; break;}
case 6:{lo=-Ol; break;}
case 7:{lo=Ol*lI; break;}
case 8:{lo=0; break;}
default:{break;}
}
if/**/( (Ol>0)&&(lI!=0)){
Ol=Ol*lI;
}
}
