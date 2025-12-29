#ifndef UTILZ_H
#define UTILZ_H

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "resource.h"
#include "../Classes/piece.h"

#include "../../Include/aiInterface.h"

using namespace Gdiplus;



Image *LoadPngFromResource(HINSTANCE hInst, int resourceID);
int min(int a, int b);





#endif