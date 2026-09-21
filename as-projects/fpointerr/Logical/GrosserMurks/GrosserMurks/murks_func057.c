#include <math.h>
#include <stdarg.h>
#include "murks_func057.h"
#include <assert.h>
#include <fenv.h>
#include <wctype.h>
void murks_func057(void){
INT lI=(INT)tanf(0.0f); 
INT Il=0;
UINT Ol=10u;
UINT lo=0;
INT oI[10]={3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
for/**/( lo=1; lo<Ol; lo++){
lI=((oI[lo]-oI[lo-1]>0)? 1 :-1);
if/**/( lI!=Il){
if/**/( (oI[lo]>5)&&(Il>0)){
oI[lo]=oI[lo]*2;
}
else if/**/( (oI[lo]<3)&&(Il<0)){
oI[lo]=0;
}
}
Il=lI;
switch/**/( oI[lo]%4){
case 0:{oI[lo]=oI[lo]/2; break;}
case 1:{oI[lo]=oI[lo]+1; break;}
case 2:{oI[lo]=oI[lo]-1; break;}
case 3:{oI[lo]=oI[lo]*2; break;}
default:{break;}
}
}
}
