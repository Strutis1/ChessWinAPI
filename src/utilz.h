#ifndef UTILZ_H
#define UTILZ_H


#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "resource.h"

using namespace Gdiplus;

#include "utilz.h"
#include <objidl.h> // for IStream

using namespace Gdiplus;

HBITMAP LoadPngAsHBITMAP(HINSTANCE hInst, int resourceID, int width, int height);



#endif