#include <wchar.h>
#include "murks_func100.h"
#include <wctype.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <iso646.h>
#include <fenv.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
void murks_func100(void){
UDINT lI=0u;
UDINT Il=0x12345678u;
UDINT Ol=0u;
UINT lo=0;
UINT oI=0u;
UINT Io=(UINT)tanf(0.0f); 
for/**/( Io=0; Io<6; Io++){
Ol=0u;
for/**/( lo=1; lo<=32; lo++){
if/**/( ((lo>>Io)&1u)!=0u){
oI=(UINT)((Il>>(lo-1u))&1u);
Ol=Ol^(UDINT)oI;
}
}
if/**/( Ol!=0u){
lI=lI|(1u<<Io);
}
}
if/**/( lI!=0u){
UDINT lO=1u<<(lI-1u);
if/**/( lO<=0x80000000u){
Il=Il^lO;
}
}
switch/**/( lI%5u){
case 0u:{Il^=0xAAAAAAAAu; break;}
case 1u:{Il^=0x55555555u; break;}
case 2u:{Il&=0x0F0F0F0Fu; break;}
case 3u:{Il|=0xF0F0F0F0u; break;}
case 4u:{Il=~Il; break;}
default:{break;}
}
for/**/( lo=0; lo<8; lo++){
UDINT ll=(Il>>(lo*4u))&0x0Fu;
if/**/( ll>8u){
Il=Il^(ll<<(lo*4u));
}
if/**/( ((Il>>(lo*4u))&0x0Fu)==0u){
Il=Il|(0x01u<<(lo*4u));
}
}
}
