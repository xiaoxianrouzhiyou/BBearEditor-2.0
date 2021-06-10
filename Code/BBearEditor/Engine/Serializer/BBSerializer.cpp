#include "BBSerializer.h"
#include <stdio.h>
#include "BBUtils.h"
#include <fstream>


bool BBSerializer::createEmptyFile(const char *pFilePath)
{
    std::ofstream file(pFilePath);
    BB_PROCESS_ERROR_RETURN_FALSE(file);
    return true;
}

bool BBSerializer::saveToFile(const char *pFilePath, void *pBuffer, int nSize)
{
    FILE *pFile = fopen(pFilePath, "wb");
    BB_PROCESS_ERROR_RETURN_FALSE(pFile);
    fwrite(pBuffer, sizeof(char), nSize, pFile);
    fclose(pFile);
    return true;
}
