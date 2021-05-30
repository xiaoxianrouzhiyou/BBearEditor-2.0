#ifndef BBFILESYSTEMDATA_H
#define BBFILESYSTEMDATA_H


#include <QMap>


class QTreeWidgetItem;
class QListWidgetItem;


enum BBFileType
{
    Dir = 0,
    Mesh = 1,
    Audio = 2,
    Texture = 3,
    Material = 4,
    Script = 5,
    Other = 6
};


struct BBFileInfo
{
    QString m_FileName;
    BBFileType m_eFileType;

    BBFileInfo(const QString &fileName, const BBFileType &eType)
    {
        m_FileName = fileName;
        m_eFileType = eType;
    }
};


typedef QMap<QListWidgetItem*, BBFileInfo*> BBFILE;


struct BBFOLDER
{
    // The path is used to traverse the subfolders
    // The item is used to insert
    QString path;
    QTreeWidgetItem *pItem;

    BBFOLDER(QString path, QTreeWidgetItem *pItem)
    {
        this->path = path;
        this->pItem = pItem;
    }
};


class BBFileSystemData
{
public:
    BBFileSystemData();
    ~BBFileSystemData();

    void load();
    QList<QTreeWidgetItem*> getFolderTreeWidgetTopLevelItems();

private:
    void buildFileData(QQueue<BBFOLDER> &queue);
    BBFILE getFolderContent(const QString &parentPath);

    QMap<QTreeWidgetItem*, BBFILE> m_TopLevelFileData;
    QMap<QTreeWidgetItem*, BBFILE> m_FileData;
};

#endif // BBFILESYSTEMDATA_H
