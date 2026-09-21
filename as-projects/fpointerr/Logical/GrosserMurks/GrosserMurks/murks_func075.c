#include <locale.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "murks_func075.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <iso646.h>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <errno.h>
#include <fenv.h>
#include <wctype.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>
void murks_func075(void){
REAL lI[3]={1.0f, 0.0f, 0.0f};
REAL Il[3]={0.0f, 1.0f, 0.0f};
REAL Ol[3]={0.0f, 0.0f, 1.0f};
REAL lo[3]={0.0f, 0.0f, 0.0f};
UINT oI=(UINT)floorf(0.0f); 
REAL Io[3], v2[3];
for/**/( oI=0; oI<3; oI++){
Io[oI]=Il[oI]-lI[oI];
v2[oI]=Ol[oI]-lI[oI];
}
lo[0]=(Io[1]*v2[2])-(Io[2]*v2[1]);
lo[1]=(Io[2]*v2[0])-(Io[0]*v2[2]);
lo[2]=(Io[0]*v2[1])-(Io[1]*v2[0]);
for/**/( oI=0; oI<3; oI++){
if/**/( lo[oI]<0.0f){
lo[oI]=-lo[oI];
}
else if/**/( lo[oI]>1.0f){
lo[oI]=1.0f;
}
}
switch/**/( (UINT)(lo[0]*10.0f+lo[1]*10.0f+lo[2]*10.0f)%4u){
case 0u:{lI[0]+=0.1f; break;}
case 1u:{Il[1]+=0.1f; break;}
case 2u:{Ol[2]+=0.1f; break;}
case 3u:{lo[0]=0.0f; lo[1]=0.0f; lo[2]=1.0f; break;}
default:{break;}
}
if/**/( (lo[0]+lo[1]+lo[2])<0.001f){
lo[2]=1.0f;
}
}
