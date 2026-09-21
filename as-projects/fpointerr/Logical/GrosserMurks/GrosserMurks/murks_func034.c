#include <locale.h>
#include <wchar.h>
#include "murks_func034.h"
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <wctype.h>
void murks_func034(void){
UINT lI=(UINT)cosf(0.0f); 
UINT Il=0u;
UINT Ol=1u;
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<8; oI++){
UINT Io=(lI>>oI)&1u;
UINT lO=(Il>>oI)&1u;
UINT ll=(Ol>>oI)&1u;
UINT OO=Io&lO;
UINT oo=Io|ll;
UINT II=lO^ll;
UINT iI=(Io!=0u)? oo : OO;
if/**/( ((OO|II)&(oo^iI))!=0u){
lo=lo|(1u<<oI);
}
}
switch/**/( lo%6u){
case 0u:{lI=0xFFu; break;}
case 1u:{Il=0xFFu; break;}
case 2u:{Ol=0x00u; break;}
case 3u:{lI^=Il; break;}
case 4u:{Il^=Ol; break;}
case 5u:{Ol^=lI; break;}
default:{break;}
}
}
