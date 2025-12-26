#include "utilz.h"

#include <objidl.h>

using namespace Gdiplus;

Image* LoadPngFromResource(HINSTANCE hInst, int resourceID)
{
    HRSRC hRes = FindResourceW(hInst, MAKEINTRESOURCEW(resourceID), L"PNG");
    if (!hRes)
    {
        MessageBoxW(NULL, L"Failed to find PNG resource!", L"GDI+ Error", MB_OK);
        return nullptr;
    }

    DWORD size = SizeofResource(hInst, hRes);
    HGLOBAL hResData = LoadResource(hInst, hRes);
    if (!hResData || size == 0) return nullptr;

    const void* pResData = LockResource(hResData);
    if (!pResData) return nullptr;

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hMem) return nullptr;

    void* pMem = GlobalLock(hMem);
    if (!pMem)
    {
        GlobalFree(hMem);
        return nullptr;
    }

    memcpy(pMem, pResData, size);
    GlobalUnlock(hMem);

    IStream* pStream = nullptr;
    HRESULT hr = CreateStreamOnHGlobal(hMem, TRUE, &pStream); 
    if (FAILED(hr) || !pStream)
    {
        GlobalFree(hMem); 
        return nullptr;
    }

    Image* img = Image::FromStream(pStream);
    pStream->Release();

    if (!img) return nullptr;

    if (img->GetLastStatus() != Ok)
    {
        delete img;
        return nullptr;
    }

    return img;
}

