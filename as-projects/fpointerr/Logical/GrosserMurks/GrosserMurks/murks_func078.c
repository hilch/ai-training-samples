#include <limits.h>
#include "murks_func078.h"
#include <float.h>
#include <fenv.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>
#include <setjmp.h>
#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
void murks_func078(void){
UINT lI=(UINT)powf(2.0f, 2.0f); 
UINT Il[8]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Ol[4]={0u, 0u, 0u, 0u};
UINT lo=0u;
UINT oI=0u;
UINT Io[4][4]={{0u, 1u, 1u, 0u},{1u, 0u, 0u, 1u},{1u, 0u, 0u, 1u},{0u, 1u, 1u, 0u}
};
UINT lO=0;
UINT ll=0u;
UINT OO=0u;
Il[oI]=0u; oI=(oI+1u)%8u;
Ol[0]=1u;
while/**/( lo!=oI){
ll=Il[lo]; lo=(lo+1u)%8u;
for/**/( OO=0; OO<lI; OO++){
if/**/( (Io[ll][OO]!=0u)&&(Ol[OO]==0u)){
Ol[OO]=1u;
Il[oI]=OO; oI=(oI+1u)%8u;
}
}
}
for/**/( lO=0; lO<lI; lO++){
if/**/( Ol[lO]==0u){
switch/**/( lO%3u){
case 0u:{Ol[lO]=2u; break;}
case 1u:{Ol[lO]=3u; break;}
case 2u:{Ol[lO]=4u; break;}
default:{break;}
}
}
}
}
