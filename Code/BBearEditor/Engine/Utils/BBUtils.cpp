#include "BBUtils.h"
#include <QDir>
#include <QProcess>
#include "FileSystem/BBFileListWidget.h"
#include <GL/gl.h>


QString BBConstant::BB_NAME_PROJECT = "";
QString BBConstant::BB_PATH_PROJECT = "";

// there is no / at the end
QString BBConstant::BB_PATH_PROJECT_ENGINE = "";
QString BBConstant::BB_PATH_PROJECT_USER = "";

QString BBConstant::BB_NAME_FILE_SYSTEM_USER = "contents";
QString BBConstant::BB_NAME_FILE_SYSTEM_ENGINE = "engine";
QString BBConstant::BB_NAME_OVERVIEW_MAP = "overview map.jpg";
QString BBConstant::BB_NAME_DEFAULT_SCENE = "new scene.bbscene";
QString BBConstant::BB_NAME_DEFAULT_MATERIAL = "new material.bbmtl";


QVector3D BBConstant::m_Red = QVector3D(0.937255f, 0.378431f, 0.164706f);
QVector4D BBConstant::m_RedTransparency = QVector4D(0.937255f, 0.378431f, 0.164706f, 0.7f);
QVector3D BBConstant::m_Green = QVector3D(0.498039f, 0.827451f, 0.25098f);
QVector4D BBConstant::m_GreenTransparency = QVector4D(0.498039f, 0.827451f, 0.25098f, 0.7f);
QVector3D BBConstant::m_Blue = QVector3D(0.341176f, 0.662745f, 1.0f);
QVector4D BBConstant::m_BlueTransparency = QVector4D(0.341176f, 0.662745f, 1.0f, 0.7f);
QVector3D BBConstant::m_Yellow = QVector3D(1.0f, 1.0f, 0.305882f);
QVector3D BBConstant::m_Gray = QVector3D(0.8f, 0.8f, 0.8f);
QVector4D BBConstant::m_GrayTransparency = QVector4D(0.8f, 0.8f, 0.8f, 0.7f);
//QVector3D BBConstant::m_Red = QVector3D(0.909804f, 0.337255f, 0.333333f);
//QVector4D BBConstant::m_RedTransparency = QVector4D(0.909804f, 0.337255f, 0.333333f, 0.5f);
//QVector3D BBConstant::m_Green = QVector3D(0.356863f, 0.729412f, 0.619608f);
//QVector4D BBConstant::m_GreenTransparency = QVector4D(0.356863f, 0.729412f, 0.619608f, 0.5f);
//QVector3D BBConstant::m_Blue = QVector3D(0.384314f, 0.631373f, 0.847059f);
//QVector4D BBConstant::m_BlueTransparency = QVector4D(0.384314f, 0.631373f, 0.847059f, 0.5f);
//QVector3D BBConstant::m_Yellow = QVector3D(0.847059f, 0.603922f, 0.309804f);
QVector3D BBConstant::m_OrangeRed = QVector3D(0.909804f, 0.337255f, 0.333333f);


char *BBUtils::loadFileContent(const char *filePath, int &nFileSize)
{
    FILE *pFile = NULL;
    char *pData = NULL;
    // Read files by binary mode
    // path.toLatin1().data(); will cause Chinese garbled
    do{
        pFile = fopen(filePath, "rb");
        BB_PROCESS_ERROR(pFile);
        // Seek the pointer to the end of the file
        BB_PROCESS_ERROR(BB_SUCCEEDED(fseek(pFile, 0, SEEK_END)));
        // Get the size of the file
        size_t length = ftell(pFile);
        BB_PROCESS_ERROR(length);
        // Seek to the beginning of the file
        BB_PROCESS_ERROR(BB_SUCCEEDED(fseek(pFile, 0, SEEK_SET)));
        // +1 Terminator
        pData = new char[length + 1];
        BB_PROCESS_ERROR(pData);
        // 1*length is the size of the file to be read
        BB_PROCESS_ERROR(fread(pData, 1, length, pFile));
        // Terminator
        pData[length] = 0;
        nFileSize = length;
    }while(0);


    if (pFile)
        fclose(pFile);

    return pData;
}

bool BBUtils::saveToFile(const char *pFilePath, void *pBuffer, int nSize)
{
    FILE *pFile = fopen(pFilePath, "wb");
    BB_PROCESS_ERROR_RETURN_FALSE(pFile);
    fwrite(pBuffer, sizeof(char), nSize, pFile);
    fclose(pFile);
    return true;
}

unsigned char* BBUtils::decodeBMP(unsigned char *pBmpFileData, int &nWidth, int &nHeight)
{
    // Is it a bitmap file
    if (0x4D42 == *((unsigned short*)pBmpFileData))
    {
        int nPixelDataOffset = *((int*)(pBmpFileData + 10));
        nWidth = *((int*)(pBmpFileData + 18));
        nHeight =  *((int*)(pBmpFileData + 22));
        unsigned char *pPixelData = pBmpFileData + nPixelDataOffset;
        // be saved as BGR, but opengl support RGB, exchange B with R
        // bmp does not support alpha
        for (int i = 0; i < nWidth * nHeight * 3; i += 3)
        {
            unsigned char temp = pPixelData[i];
            pPixelData[i] = pPixelData[i + 2];
            pPixelData[i + 2] = temp;
        }
        return pPixelData;
    }
    return nullptr;
}

unsigned int BBUtils::getBlendFunc(int nIndex)
{
    unsigned int func = GL_ZERO;
    switch (nIndex) {
    case 0:
        func = GL_ZERO;
        break;
    case 1:
        func = GL_ONE;
        break;
    case 2:
        func = GL_SRC_COLOR;
        break;
    case 3:
        func = GL_ONE_MINUS_SRC_COLOR;
        break;
    case 4:
        func = GL_SRC_ALPHA;
        break;
    case 5:
        func = GL_ONE_MINUS_SRC_ALPHA;
        break;
    case 6:
        func = GL_DST_ALPHA;
        break;
    case 7:
        func = GL_ONE_MINUS_DST_ALPHA;
        break;
    default:
        break;
    }
    return func;
}

QString BBUtils::getBlendFuncName(unsigned int func)
{
    QString name;
    switch (func) {
    case GL_ZERO:
        name = "GL_ZERO";
        break;
    default:
        break;
    }
    return name;
}
