#include "BBProcedureTexture.h"


BBProcedureTexture::BBProcedureTexture()
    : BBTexture()
{

}

GLuint BBProcedureTexture::create0(int nSize)
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

GLuint BBProcedureTexture::create3D0(int nWidth, int nHeight, int nDepth)
{
    char *pData = new char[nWidth * nHeight * nDepth * 4];
    char *pCurrent = pData;
    for (int a = 0; a < nWidth; a++)
    {
        for (int b = 0; b < nHeight; b++)
        {
            for (int c = 0; c < nDepth; c++)
            {
                *pCurrent++ = rand() & 0xff;
                *pCurrent++ = rand() & 0xff;
                *pCurrent++ = rand() & 0xff;
                *pCurrent++ = rand() & 0xff;
            }
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8_SNORM, nWidth, nHeight, nDepth, 0, GL_RGBA, GL_BYTE, pData);
    glBindTexture(GL_TEXTURE_3D, 0);
    BB_SAFE_DELETE(pData);
    return texture;
}
