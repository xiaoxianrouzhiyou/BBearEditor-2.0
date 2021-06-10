#ifndef BBSERIALIZER_H
#define BBSERIALIZER_H


class BBSerializer
{
public:
    static bool createEmptyFile(const char *pFilePath);
    static bool saveToFile(const char *pFilePath, void *pBuffer, int nSize);
};

#endif // BBSERIALIZER_H
