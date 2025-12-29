#ifndef AILOADER_H
#define AILOADER_H

#include "aiInterface.h"

using ChooseMoveFromListFn = int (*)(const AIBoard*, const AIMoveList*, AIMove*);

void SetAiChooseMoveFromListFn(ChooseMoveFromListFn fn);
ChooseMoveFromListFn GetAiChooseMoveFromListFn();

#endif
