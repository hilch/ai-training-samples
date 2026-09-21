#include "murks_func065.h"
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
void murks_func065(void){
INT lI[4]={10, 20, 30, 40};
INT Il[4]={100,-50, 75,-25};
INT Ol[4]={0, 0, 0, 0};
INT lo=(INT)fabsf(7.0f); 
UINT oI=0;
for/**/( oI=0; oI<4; oI++){
Ol[oI]=(lo*Il[oI]+(10-lo)*lI[oI])/10;
if/**/( (Ol[oI]>0)&&(Il[oI]<0)){
Ol[oI]=-Ol[oI];
}
else if/**/( (Ol[oI]<0)&&(Il[oI]>0)){
Ol[oI]=-Ol[oI];
}
lI[oI]=Ol[oI];
}
switch/**/( Ol[3]%5){
case 0:{lo=5; break;}
case 1:{lo=7; break;}
case 2:{lo=3; break;}
case 3:{lo=9; break;}
case 4:{lo=1; break;}
default:{break;}
}
for/**/( oI=0; oI<4; oI++){
if/**/( (Ol[oI]>50)||(Ol[oI]<-50)){
Ol[oI]=Ol[oI]/2;
}
}
}
