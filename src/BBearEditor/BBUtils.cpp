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




//bool BBUtils::showInFolder(const QString &filePath)
//{
//    BB_PROCESS_ERROR_RETURN_FALSE(!filePath.isEmpty());
//    QProcess process;
//    // just identify "\\"
//    QString legalPath = filePath;
//    legalPath.replace("/", "\\");
//    QString cmd = QString("explorer.exe /select,%1").arg(legalPath);
//    qDebug() << cmd;
//    return process.startDetached(cmd);
//}

//bool BBUtils::copyFolder(const QString &fromDir, const QString &toDir)
//{
//    QDir sourceDir(fromDir);
//    QDir targetDir(toDir);
//    if (!targetDir.exists())
//    {
//        // if it does not exist, create it
//        BB_PROCESS_ERROR_RETURN_FALSE(targetDir.mkpath(targetDir.absolutePath()));
//    }

//    QFileInfoList fileInfoList = sourceDir.entryInfoList();
//    foreach (QFileInfo fileInfo, fileInfoList)
//    {
//        // fileInfo.filePath() has /./
//        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
//            continue;

//        if (fileInfo.isDir())
//        {
//            // recursive
//            BB_PROCESS_ERROR_RETURN_FALSE(copyFolder(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
//        }
//        else
//        {
//            // copy files
//            BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())));
////            //材质文件需要新建材质对象
////            QString suffix = fileInfo.fileName().mid(fileInfo.fileName().lastIndexOf('.') + 1);
////            if (suffix == "mtl")
////            {
////                new Material(fileInfo.absoluteFilePath());
////            }
//        }
//    }
//    return true;
//}

//bool BBUtils::isMovablePath(const QString &sourcePath, const QString &destParentPath)
//{
//    // Folders can’t be moved into oneself, nor can they be moved into their own subfolders
//    BB_PROCESS_ERROR_RETURN_FALSE(!(destParentPath.mid(0, sourcePath.length()) == sourcePath));
//    // It can’t become its own brother and move into its own parent folder
//    BB_PROCESS_ERROR_RETURN_FALSE(!(BBUtils::getParentPath(sourcePath) == destParentPath));
//    return true;
//}

//bool BBUtils::moveFolder(const QString &oldPath, const QString &newPath, bool bCopy)
//{
//    // newPath has been checked for duplicate name problem
//    // copy folder
//    BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::copyFolder(oldPath, newPath));
//    // handle corresponding folder in the engine folder
//    QString oldAuxiliaryFolderPath = BBUtils::getEngineAuxiliaryFolderPath(oldPath);
//    QString newAuxiliaryFolderPath = BBUtils::getEngineAuxiliaryFolderPath(newPath);
//    BB_PROCESS_ERROR_RETURN_FALSE(BBUtils::copyFolder(oldAuxiliaryFolderPath, newAuxiliaryFolderPath));

//    if (!bCopy)
//    {
//        // delete original folder
//        QDir dir(oldPath);
//        BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
//        dir = QDir(oldAuxiliaryFolderPath);
//        BB_PROCESS_ERROR_RETURN_FALSE(dir.removeRecursively());
//    }
//}

//bool BBUtils::moveFile(const QString &oldPath, const QString &newPath, BBFileType eFileType, bool bCopy)
//{
//    // newPath has been checked for duplicate name problem
//    // copy file
//    BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(oldPath, newPath));
//    // handle corresponding folder in the engine folder
//    QString oldOverviewMapPath;
//    QString newOverviewMapPath;
//    if (eFileType == BBFileType::mesh)
//    {
//        oldOverviewMapPath = BBUtils::getOverviewMapPath(oldPath);
//        newOverviewMapPath = BBUtils::getOverviewMapPath(newPath);
//        BB_PROCESS_ERROR_RETURN_FALSE(QFile::copy(oldOverviewMapPath, newOverviewMapPath));
//    }
////    //材质文件需要新建材质对象
////    else if (fileInfo->mFileType == FileType::material)
////    {
////        new Material(newPath);
////    }

//    if (!bCopy)
//    {
//        // delete original file
//        BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(oldPath));
//        BB_PROCESS_ERROR_RETURN_FALSE(QFile::remove(oldOverviewMapPath));
//    }
//}
