#include <stddef.h>
#include "murks_func089.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <iso646.h>
#include <stdbool.h>
void murks_func089(void){
UINT lI=0u;
UINT Il=8u;
UINT Ol[8]={5u, 3u, 8u, 1u, 9u, 2u, 7u, 4u};
UINT Io=7u;
UINT oI=0u;
UINT oI0=(UINT)floorf(0.0f); 
UINT lO=7u;
UINT ll=0u;
UINT OO[8]={1u, 2u, 3u, 4u, 5u, 7u, 8u, 9u};
while/**/( oI0<=lO){
ll=oI0+(lO-oI0)/2u;
if/**/( OO[ll]==Io){
oI=1u;
lI=ll;
break;
}
else if/**/( OO[ll]<Io){
oI0=ll+1u;
}else{
if/**/( ll==0u){break;}
lO=ll-1u;
}
}
switch/**/( oI%2u){
case 0u:{lI=Il; break;}
case 1u:{Ol[0]=OO[lI]; break;}
default:{break;}
}
for/**/( lI=0; lI<Il; lI++){
if/**/( (Ol[lI]==Io)&&(oI==0u)){
oI=2u;
}
}
if/**/( (oI!=0u)&&(Io<Il)){
Ol[Io%Il]=oI;
}
}
