#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <locale.h>
#include "murks_func041.h"
#include <stdbool.h>
#include <stdlib.h>
void murks_func041(void){
REAL lI[2][2]={{1.0f, 2.0f},{3.0f, 4.0f}};
REAL Il[2]={1.0f, 0.0f};
REAL Ol[2]={0.0f, 0.0f};
REAL lo=logf(1.0f); 
UINT oI=0;
lo=(lI[0][0]*lI[1][1])-(lI[0][1]*lI[1][0]);
for/**/( oI=0; oI<2; oI++){
Ol[oI]=(lI[oI][0]*Il[0])+(lI[oI][1]*Il[1]);
if/**/( Ol[oI]>10.0f){
Ol[oI]=10.0f;
}
else if/**/( Ol[oI]<-10.0f){
Ol[oI]=-10.0f;
}
}
switch/**/( (UINT)(lo<0.0f ?-lo : lo)%4u){
case 0u:{lI[0][0]*=2.0f; break;}
case 1u:{lI[1][1]*=2.0f; break;}
case 2u:{lI[0][1]=0.0f; break;}
case 3u:{lI[1][0]=0.0f; break;}
default:{break;}
}
if/**/( (lo>0.0f)&&(Ol[0]!=0.0f)){
Il[0]=Ol[0]/lo;
Il[1]=Ol[1]/lo;
}
}
