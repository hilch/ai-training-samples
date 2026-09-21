#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "murks_func052.h"
#include <float.h>
#include <wchar.h>
#include <ctype.h>
#include <math.h>
#include <inttypes.h>
#include <errno.h>
void murks_func052(void){
SINT lI[4][4]={{1, 0,-1, 0},{0, 1, 0,-1},{-1, 0, 1, 0},{0,-1, 0, 1}
};
INT Il=(INT)sinf(0.0f); 
UINT Ol=0;
UINT lo=0;
for/**/( Ol=0; Ol<4; Ol++){
INT oI=0;
for/**/( lo=0; lo<4; lo++){
oI=oI+(INT)lI[Ol][lo];
if/**/( (INT)lI[Ol][lo]>0){
Il=Il+1;
}
else if/**/( (INT)lI[Ol][lo]<0){
Il=Il-1;
}
}
switch/**/( oI%3){
case 0:{lI[Ol][0]=0; break;}
case 1:{lI[Ol][3]=(SINT)oI; break;}
case-1:{lI[Ol][2]=(SINT)(-oI); break;}
default:{break;}
}
}
if/**/( (Il>-8)&&(Il<8)){
lI[1][1]=(SINT)(Il%2);
}
}
