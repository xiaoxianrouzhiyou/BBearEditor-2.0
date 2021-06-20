#include "BBUtils.h"
#include <QDir>
#include <QProcess>
#include "FileSystem/BBFileListWidget.h"


QString BBConstant::BB_NAME_PROJECT = "";
QString BBConstant::BB_PATH_PROJECT = "";

// there is no / at the end
QString BBConstant::BB_PATH_PROJECT_ENGINE = "";
QString BBConstant::BB_PATH_PROJECT_USER = "";

QString BBConstant::BB_NAME_FILE_SYSTEM_USER = "contents";
QString BBConstant::BB_NAME_FILE_SYSTEM_ENGINE = "engine";
QString BBConstant::BB_NAME_OVERVIEW_MAP = "overview map.jpg";
QString BBConstant::BB_NAME_DEFAULT_SCENE = "new scene.bbscene";


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


