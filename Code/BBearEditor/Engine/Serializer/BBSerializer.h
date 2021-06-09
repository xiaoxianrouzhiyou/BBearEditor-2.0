#ifndef BBSERIALIZER_H
#define BBSERIALIZER_H


class BBSerializer
{
public:
    static void saveToFile(const char *pFilePath, void *pBuffer, int nSize);
};

#endif // BBSERIALIZER_H
