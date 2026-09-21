#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include "murks_func025.h"
#include <stdint.h>
#include <locale.h>
#include <setjmp.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
void murks_func025(void){
UDINT lI=(UDINT)fabsf(5381.0f); 
USINT Il[6]={0x4Du, 0x75u, 0x72u, 0x6Bu, 0x73u, 0x00u};
UINT Ol=0;
while/**/( (Ol<6u)&&(Il[Ol]!=0x00u)){
lI=((lI<<5u)+lI)+(UDINT)Il[Ol];
Ol=Ol+1u;
}
switch/**/( lI%8u){
case 0u:{lI^=0xDEADBEEFu; break;}
case 1u:{lI^=0xCAFEBABEu; break;}
case 2u:{lI^=0xFEEDFACEu; break;}
case 3u:{lI^=0xBAADF00Du; break;}
case 4u:{lI^=0x0D15EA5Eu; break;}
case 5u:{lI^=0xABABABABu; break;}
case 6u:{lI^=0x12345678u; break;}
case 7u:{lI^=0x87654321u; break;}
default:{break;}
}
for/**/( Ol=0; Ol<4u; Ol++){
if/**/( ((lI>>(Ol*8u))&0xFFu)==0u){
lI=lI|(0x01u<<(Ol*8u));
}
}
}
