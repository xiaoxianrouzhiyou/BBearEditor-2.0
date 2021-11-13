#include "BBProcedureTexture.h"
#include "Math/BBPerlinNoise.h"


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
            pImageData[nCurrentPixelOffset + 0] = 255;
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
    unsigned char *pData = new unsigned char[nWidth * nHeight * nDepth * 4];
    unsigned char *pCurrent = pData;
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

    GLuint texture = createTexture3D(pData, nWidth, nHeight, nDepth, GL_RGBA);
    BB_SAFE_DELETE(pData);
    return texture;
}

GLuint BBProcedureTexture::createPerlinNoiseTexture2D(int nSize, float fScale)
{
    unsigned char *pImageData = new unsigned char[nSize * nSize * 4];
    for (int y = 0; y < nSize; y++)
    {
        for (int x = 0; x < nSize; x++)
        {
            int nCurrentPixelOffset = (x + y * nSize) * 4;
            float fNoise = BBPerlinNoise::getNoise(QVector3D(x, y, 0) * fScale);
            // 0~1 -> 0~255;
            fNoise *= 255.0f;
            pImageData[nCurrentPixelOffset + 0] = fNoise;
            pImageData[nCurrentPixelOffset + 1] = fNoise;
            pImageData[nCurrentPixelOffset + 2] = fNoise;
            pImageData[nCurrentPixelOffset + 3] = 255.0f;
        }
    }

    GLuint texture = createTexture2D(pImageData, nSize, nSize, GL_RGBA);
    BB_SAFE_DELETE_ARRAY(pImageData);
    return texture;
}

GLuint BBProcedureTexture::createCamouflagePerlinNoiseTexture2D(int nSize, float fScale)
{
    unsigned char *pImageData = new unsigned char[nSize * nSize * 4];
    for (int y = 0; y < nSize; y++)
    {
        for (int x = 0; x < nSize; x++)
        {
            int nCurrentPixelOffset = (x + y * nSize) * 4;
            float fNoise = BBPerlinNoise::getTurbulenceNoise(QVector3D(x, y, 0) * fScale);
            fNoise =
            // 0~1 -> 0~255;
            fNoise *= 255.0f;
            pImageData[nCurrentPixelOffset + 0] = fNoise;
            pImageData[nCurrentPixelOffset + 1] = fNoise;
            pImageData[nCurrentPixelOffset + 2] = fNoise;
            pImageData[nCurrentPixelOffset + 3] = 255.0f;
        }
    }

    GLuint texture = createTexture2D(pImageData, nSize, nSize, GL_RGBA);
    BB_SAFE_DELETE_ARRAY(pImageData);
    return texture;
}

GLuint BBProcedureTexture::createPerlinNoiseTexture3D(int nWidth, int nHeight, int nDepth, float fScale)
{
    unsigned char *pData = new unsigned char[nWidth * nHeight * nDepth * 4];
    for (int z = 0; z < nDepth; z++)
    {
        for (int y = 0; y < nHeight; y++)
        {
            for (int x = 0; x < nWidth; x++)
            {
                int nCurrentPixelOffset = (x + y * nWidth + z * nWidth * nHeight) * 4;
                float fNoise = BBPerlinNoise::getNoise(QVector3D(x, y, z) * fScale);

                // 0~1 -> 0~255;
                fNoise *= 255.0f;
                pData[nCurrentPixelOffset + 0] = fNoise;
                pData[nCurrentPixelOffset + 1] = fNoise;
                pData[nCurrentPixelOffset + 2] = fNoise;
                pData[nCurrentPixelOffset + 3] = 255.0f;
            }
        }
    }

    GLuint texture = createTexture3D(pData, nWidth, nHeight, nDepth, GL_RGBA);
    BB_SAFE_DELETE(pData);
    return texture;
}
