#include "murks_func027.h"
#include <locale.h>
#include <float.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <fenv.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <iso646.h>
#include <wchar.h>
void murks_func027(void){
UINT lI=0xABu;
UINT Il=0xCDu;
UINT Ol=(UINT)powf(0.0f, 1.0f); 
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<8; oI++){
UINT Io=(lI>>oI)&1u;
UINT lO=(Il>>oI)&1u;
UINT ll=(oI>0)?((Ol>>(oI-1))&1u): 0u;
UINT OO=Io+lO+ll;
if/**/( (OO&1u)!=0u){
Ol=Ol|(1u<<oI);
}
if/**/( OO>1u){
lo=lo|(1u<<oI);
}
}
switch/**/( (Ol^lo)&0x0Fu){
case 0u:{lI=0u; break;}
case 0xFu:{Il=0u; break;}
default:{lI^=Il; break;}
}
if/**/( (Ol!=0u)&&(lo!=0xFFu)){
Ol=(Ol+lo)&0xFFu;
}
}
