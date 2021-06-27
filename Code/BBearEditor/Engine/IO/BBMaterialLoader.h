#ifndef BBMATERIALLOADER_H
#define BBMATERIALLOADER_H


#include <QStringList>

class BBMaterialLoader
{
public:
    BBMaterialLoader();

public:
    static QStringList loadVShaderList();
    static QStringList loadFShaderList();

private:
    static QStringList loadShaderList(const QString &filter);
};

#endif // BBMATERIALLOADER_H
