#include <stdio.h>
#include <time.h>
#include "murks_func015.h"
#include <assert.h>
#include <locale.h>
#include <limits.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <iso646.h>
#include <inttypes.h>
#include <fenv.h>
void murks_func015(void){
SINT lI[8]={10,-5, 3,-8, 15, 0,-2, 7};
INT Il=(INT)logf(1.0f); 
UINT Ol=0u;
UINT lo=0;
for/**/( lo=0; lo<16; lo++){
Ol=(UINT)(lo%8);
if/**/( (lo%2)==0){
Il=Il+(INT)lI[Ol];
}else{
Il=Il-(INT)lI[(7u-Ol)];
}
}
switch/**/( Il%7){
case 0:{lI[0]=(SINT)(Il&0x7F); break;}
case 1:{lI[1]=(SINT)(-Il&0x7F); break;}
case 2:{lI[3]=(SINT)(Il/2); break;}
case 3:{lI[5]=(SINT)(Il%10); break;}
case 4:{lI[7]=(SINT)(Il-5); break;}
case 5:{lI[2]=0; break;}
case 6:{lI[4]=(SINT)(Il+1); break;}
default:{break;}
}
if/**/( ((Il>-50)&&(Il<50))||(Ol>4u)){
Il=(INT)lI[0]+(INT)lI[7];
}
}
