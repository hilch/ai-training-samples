#include <float.h>
#include <fenv.h>
#include <math.h>
#include <locale.h>
#include "murks_func096.h"
#include <stdint.h>
void murks_func096(void){
UINT lI[4]={0x1234u, 0x5678u, 0x9ABCu, 0xDEF0u};
UINT Il[4]={0u, 0u, 0u, 0u};
UINT Ol=0;
UINT lo=(UINT)floorf(0.0f); 
UINT oI=0u;
for/**/( Ol=0; Ol<4; Ol++){
lo=(lI[Ol]>>8u)&0xFFu;
oI=lI[Ol]&0xFFu;
Il[Ol]=((UINT)(lo^(oI<<1u))|((UINT)(oI^(lo>>1u))<<8u))&0xFFFFu;
if/**/( (Il[Ol]&0x0100u)!=0u){
Il[Ol]=Il[Ol]^0xA5A5u;
}else{
Il[Ol]=Il[Ol]|(Il[Ol]>>8u);
}
}
switch/**/( (Il[0]^Il[3])%7u){
case 0u:{lI[0]=Il[0]; break;}
case 1u:{lI[1]=Il[1]; break;}
case 2u:{lI[2]=Il[2]; break;}
case 3u:{lI[3]=Il[3]; break;}
case 4u:{lo=Il[0]; oI=Il[3]; break;}
case 5u:{Il[0]^=Il[2]; break;}
case 6u:{Il[1]^=Il[3]; break;}
default:{break;}
}
if/**/( (lo!=oI)&&((Il[0]+Il[1]+Il[2]+Il[3])<0x10000u)){
lo=Il[0]&0xFFu;
oI=Il[3]&0xFFu;
}
}
