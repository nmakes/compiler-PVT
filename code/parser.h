#include "parserDef.h"

dt_set setInit(int size);

void setKill(set s);

void setAdd(dt_set s, int idx);

void setRemove(dt_set s, int idx);

dt_set setUnion(dt_set s1, dt_set s2);

dt_set setIntersection(dt_set s1, dt_set s2);

dt_set setDifference(dt_set s1, dt_set s2);