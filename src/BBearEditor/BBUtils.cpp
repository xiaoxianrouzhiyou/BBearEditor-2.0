#include "BBUtils.h"
#include <QDir>
#include <QProcess>
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

QString BBUtils::getExclusiveFolderPath(const QString &filePath)
{
    QDir dir;
    if (dir.exists(filePath))
    {
        // if exist, number that is at the end will increase
        int i = 2;
        while (dir.exists(filePath + " " + QString::number(i)))
        {
            i++;
        }
        return filePath + " " + QString::number(i);
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

QString BBUtils::getParentPath(const QString &filePath)
{
    return filePath.mid(0, filePath.lastIndexOf('/'));
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

bool BBUtils::showInFolder(const QString &filePath)
{
    BB_PROCESS_ERROR_RETURN_FALSE(!filePath.isEmpty());
    QProcess process;
    // just identify "\\"
    QString legalPath = filePath;
    legalPath.replace("/", "\\");
    QString cmd = QString("explorer.exe /select,%1").arg(legalPath);
    qDebug() << cmd;
    return process.startDetached(cmd);
}

bool BBUtils::copyFolder(const QString &fromDir, const QString &toDir)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists())
    {
        // if it does not exist, create it
        BB_PROCESS_ERROR_RETURN_FALSE(targetDir.mkpath(targetDir.absolutePath()));
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        // fileInfo.filePath() has /./
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir())
        {
            // recursive
            BB_PROCESS_ERROR_RETURN_FALSE(copyFolder(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
        }
        else
        {
            // copy files
            BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
//            //材质文件需要新建材质对象
//            QString suffix = fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
//            if (suffix == "mtl")
//            {
//                new Material(fileInfo.absoluteFilePath());
//            }
        }
    }
    return true;
}

bool BBUtils::moveFolder(const QString &oldPath, const QString &newPath, bool bCopy)
{
    // newPath has been checked for duplicate name problem
    // copy folder
    BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::copyFolder(oldPath, newPath));
    // handle corresponding folder in the engine folder
    QString oldAuxiliaryFolderPath = BBUtils::getEngineAuxiliaryFolderPath(oldPath);
    QString newAuxiliaryFolderPath = BBUtils::getEngineAuxiliaryFolderPath(newPath);
    BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::copyFolder(oldAuxiliaryFolderPath, newAuxiliaryFolderPath));

    if (!bCopy)
    {
        // delete original folder
        QDir dir(oldPath);
        BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
        dir = QDir(oldAuxiliaryFolderPath);
        BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
    }
}

bool BBUtils::moveFile(const QString &oldPath, const QString &newPath, BBFileType eFileType, bool bCopy)
{
    // newPath has been checked for duplicate name problem
    // copy file
    BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(oldPath, newPath));
    // handle corresponding folder in the engine folder
    QString oldOverviewMapPath;
    QString newOverviewMapPath;
    if (eFileType == BBFileType::mesh)
    {
        oldOverviewMapPath = BBUtils::getOverviewMapPath(oldPath);
        newOverviewMapPath = BBUtils::getOverviewMapPath(newPath);
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(oldOverviewMapPath, newOverviewMapPath));
    }

    if (!bCopy)
    {
        // delete original file
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(oldPath));
        BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(oldOverviewMapPath));
    }
}
