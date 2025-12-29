#include "../../Include/aiLoader.h"

static ChooseMoveFromListFn gChooseMoveFromList = nullptr;

void SetAiChooseMoveFromListFn(ChooseMoveFromListFn fn)
{
    gChooseMoveFromList = fn;
}

ChooseMoveFromListFn GetAiChooseMoveFromListFn()
{
    return gChooseMoveFromList;
}
