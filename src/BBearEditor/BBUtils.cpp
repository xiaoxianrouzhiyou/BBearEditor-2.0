#include "BBUtils.h"
#include <QDir>
#include "FileSystem/BBFileListWidget.h"


QString BBConstant::BB_NAME_PROJECT = "";
QString BBConstant::BB_PATH_PROJECT = "";

// there is no / at the end
QString BBConstant::BB_PATH_PROJECT_ENGINE = "";
QString BBConstant::BB_PATH_PROJECT_USER = "";

QString BBConstant::BB_PATH_RESOURCE_PICTURE = "../../../BBearEditor/resources/pictures/";
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

QString BBUtils::getExclusiveFolderPath(const QString &parentPath, QString &fileName)
{
    QDir dir;
    QString filePath = parentPath + "/" + fileName;
    if (dir.exists(filePath))
    {
        // if exist, number that is at the end will increase
        filePath += " ";
        int i = 2;
        while (dir.exists(filePath + QString::number(i)))
        {
            i++;
        }
        fileName = fileName + " " + QString::number(i);
        return filePath + QString::number(i);
    }
    else
    {
        // there is no the same name
        return filePath;
    }
}

QString BBUtils::getExclusiveFilePath(const QString &parentPath, QString &fileName)
{
    QFile file;
    QString filePath = parentPath + "/" + fileName;
    if (file.exists(filePath))
    {
        QString suffix = getFileSuffix(fileName);
        QString baseName = getBaseName(fileName);
        filePath = parentPath + "/" + baseName + " ";
        int i = 2;
        while (file.exists(filePath + QString::number(i) + "." + suffix))
        {
            i++;
        }
        fileName = baseName + " " + QString::number(i) + "." + suffix;
        return filePath + QString::number(i) + "." + suffix;
    }
    else
    {
        return filePath;
    }
}

QString BBUtils::getExclusiveFilePath(const QString &filePath)
{
    QFile file;
    if (file.exists(filePath))
    {
        QString fileName = getFileNameByPath(filePath);
        QString suffix = getFileSuffix(fileName);
        // remove suffix
        QString newPath = filePath.mid(0, filePath.lastIndexOf('.'));
        newPath += " ";
        int i = 2;
        while (file.exists(newPath + QString::number(i) + "." + suffix))
        {
            i++;
        }
        return newPath + QString::number(i) + "." + suffix;
    }
    else
    {
        return filePath;
    }
}

QString BBUtils::getFileSuffix(const QFileInfo &fileInfo)
{
    return fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
}

QString BBUtils::getFileSuffix(const QString &name)
{
    return name.mid(name.lastIndexOf('.') + 1);
}

QString BBUtils::getBaseName(const QString &name)
{
    return name.mid(0, name.lastIndexOf('.'));
}

QString BBUtils::getFileNameByPath(const QString &filePath)
{
    return filePath.mid(filePath.lastIndexOf('/') + 1);
}

QString BBUtils::getEngineAuxiliaryFolderPath(const QString &sourcePath)
{
    // the path relative to the engine folder is the same as the path relative to the contents folder
    QString relativePath = sourcePath.mid(BBConstant::BB_PATH_PROJECT_USER.length());

    return BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER + relativePath;
}

QString BBUtils::getOverviewMapPath(const QString &sourcePath)
{
    QString fileName = getFileNameByPath(sourcePath);
    QString suffix = getFileSuffix(fileName);
    QString baseName = getBaseName(fileName);
    if (BBFileListWidget::m_MeshSuffixs.contains(suffix))
    {
        fileName = "mesh_" + baseName + ".jpg";
    }
    else if (BBFileListWidget::m_MaterialSuffixs.contains(suffix))
    {
        fileName = "material_" + baseName + ".jpg";
    }

    // remove the name of sourcePath, there is a '/' at the end for the convenience of calculation
    QString relativePath = sourcePath.mid(0, sourcePath.lastIndexOf('/') + 1);
    // the path relative to the engine folder is the same as the path relative to the contents folder
    relativePath = relativePath.mid(BBConstant::BB_PATH_PROJECT_USER.length() + 1);

    relativePath = relativePath + fileName;

    return BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER + "/" + relativePath;
}
