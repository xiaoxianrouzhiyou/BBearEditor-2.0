#include "BBUtils.h"


QString BBConstant::BB_PATH_PROJECT = "";
QString BBConstant::BB_PATH_PROJECT_ENGINE = "";
QString BBConstant::BB_PATH_RESOURCE_PICTURES = "../../../BBearEditor/resources/pictures/";
QString BBConstant::BB_NAME_FILE_SYSTEM_USER = "contents";
QString BBConstant::BB_NAME_FILE_SYSTEM_ENGINE = "engine";
QString BBConstant::BB_NAME_OVERVIEW_MAP = "overview map.jpg";


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
