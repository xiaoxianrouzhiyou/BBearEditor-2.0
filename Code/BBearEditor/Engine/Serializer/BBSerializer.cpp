#include "BBSerializer.h"
#include <stdio.h>
#include "BBUtils.h"


void BBSerializer::saveToFile(const char *pFilePath, void *pBuffer, int nSize)
{
    FILE *pFile = fopen(pFilePath, "wb");
    BB_PROCESS_ERROR_RETURN(pFile);
    fwrite(pBuffer, sizeof(char), nSize, pFile);
    fclose(pFile);
}
