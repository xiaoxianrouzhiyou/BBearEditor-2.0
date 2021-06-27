#include "BBMaterialLoader.h"
#include <QDir>
#include "Utils/BBUtils.h"
#include <QDirIterator>


BBMaterialLoader::BBMaterialLoader()
{

}

QStringList BBMaterialLoader::loadVShaderList()
{
    return loadShaderList("*.vert");
}

QStringList BBMaterialLoader::loadFShaderList()
{
    return loadShaderList("*.frag");
}

QStringList BBMaterialLoader::loadShaderList(const QString &filter)
{
    QStringList nameList;
    QDir dir;
    QStringList filters;
    filters << filter;
    dir.setPath(BB_PATH_RESOURCE_SHADER());
    dir.setNameFilters(filters);
    QDirIterator iter(dir, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo info = iter.fileInfo();
        if (info.isFile())
        {
            nameList.append(info.baseName());
        }
    }
    return nameList;
}
