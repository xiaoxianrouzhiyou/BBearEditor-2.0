#include "BBProcedureTexture.h"


BBProcedureTexture::BBProcedureTexture()
    : BBTexture()
{

}

GLuint BBProcedureTexture::create(int nSize)
{
    unsigned char *pImageData = new unsigned char[nSize * nSize * 4];
    float fHalfSize = (float)nSize / 2.0f;
    float fMaxDistance = sqrtf(fHalfSize * fHalfSize + fHalfSize * fHalfSize);
    float fCenterX = fHalfSize;
    float fCenterY = fHalfSize;
    for (int y = 0; y < nSize; y++)
    {
        for (int x = 0; x < nSize; x++)
        {
            int nCurrentPixelOffset = (x + y * nSize) * 4;
            pImageData[nCurrentPixelOffset] = 255;
            pImageData[nCurrentPixelOffset + 1] = 255;
            pImageData[nCurrentPixelOffset + 2] = 255;
            float fDeltaX = (float)x - fCenterX;
            float fDeltaY = (float)y - fCenterY;
            float fDistance = sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY);
            float fAlpha = powf(1.0f - (fDistance / fMaxDistance), 8.0f);
            fAlpha = fAlpha > 1.0f ? 1.0f : fAlpha;
            pImageData[nCurrentPixelOffset + 3] = (unsigned char)(fAlpha * 255.0f);
        }
    }
    GLuint texture = createTexture2D(pImageData, nSize, nSize, GL_RGBA);
    BB_SAFE_DELETE_ARRAY(pImageData);
    return texture;
}
