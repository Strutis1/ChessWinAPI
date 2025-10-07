#include "utilz.h"

#include <objidl.h>

using namespace Gdiplus;

Image *LoadPngFromResource(HINSTANCE hInst, int resourceID)
{
    HRSRC hRes = FindResourceW(hInst, MAKEINTRESOURCEW(resourceID), L"PNG");
    if (!hRes)
    {
        MessageBoxW(NULL, L"Failed to resource!", L"GDI+ Error", MB_OK);
        return nullptr;
    }

    DWORD size = SizeofResource(hInst, hRes);
    const void *pResData = LockResource(LoadResource(hInst, hRes));

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    memcpy(GlobalLock(hMem), pResData, size);
    GlobalUnlock(hMem);

    IStream *pStream = nullptr;
    CreateStreamOnHGlobal(hMem, TRUE, &pStream);

    Image *img = Image::FromStream(pStream);
    pStream->Release();

    return img;
}
