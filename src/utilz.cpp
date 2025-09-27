#include "utilz.h"

#include <objidl.h> 

using namespace Gdiplus;

HBITMAP LoadPngAsHBITMAP(HINSTANCE hInst, int resourceID, int width, int height)
{
    HRSRC hResource = FindResourceW(hInst, MAKEINTRESOURCEW(resourceID), L"PNG");
    if (!hResource) return NULL;

    DWORD imageSize = SizeofResource(hInst, hResource);
    const void* pResourceData = LockResource(LoadResource(hInst, hResource));
    if (!pResourceData) return NULL;

    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (!hBuffer) return NULL;

    void* pBuffer = GlobalLock(hBuffer);
    memcpy(pBuffer, pResourceData, imageSize);
    GlobalUnlock(hBuffer);

    IStream* pStream = nullptr;
    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK)
    {
        GlobalFree(hBuffer);
        return NULL;
    }

    Image* image = Image::FromStream(pStream);
    pStream->Release();

    if (!image) return NULL;

    HDC screenDC = GetDC(NULL);
    HBITMAP hbm = CreateCompatibleBitmap(screenDC, width, height);
    HDC memDC = CreateCompatibleDC(screenDC);
    HBITMAP old = (HBITMAP)SelectObject(memDC, hbm);

    {
        Graphics graphics(memDC);
        graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        graphics.DrawImage(image, 0, 0, width, height); // scale once
    }

    SelectObject(memDC, old);
    DeleteDC(memDC);
    ReleaseDC(NULL, screenDC);
    delete image;

    return hbm;
}
