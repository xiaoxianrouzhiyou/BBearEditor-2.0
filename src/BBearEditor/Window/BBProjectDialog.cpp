#include "BBProjectDialog.h"
#include "ui_BBProjectDialog.h"
#include "BBUtils.h"
#include <QSettings>
#include <QDir>


QString BBProjectDialog::m_ProjectDirArrayKey = "ProjectDirArrayKey";
QString BBProjectDialog::m_ProjectDirKey = "ProjectDirKey";

BBProjectDialog::BBProjectDialog(QWidget *pMainWidget, QWidget *pParent)
    : QDialog(pParent),
      m_pUi(new Ui::BBProjectDialog)
{
    m_pUi->setupUi(this);

    QObject::connect(m_pUi->buttonClose, SIGNAL(clicked()), this, SLOT(closeDialog()));

    m_pMainWidget = pMainWidget;
    setButtonTabBar();
    loadExistingProject();
    setListWidget();
    setLineEdit();
}

BBProjectDialog::~BBProjectDialog()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBProjectDialog::switchDiskAndCloud(QAbstractButton *pButton)
{
    if (pButton->objectName() == "buttonDisk")
    {
        m_pUi->stackedDiskAndCloud->setCurrentIndex(0);
    }
    else if (pButton->objectName() == "buttonCloud")
    {
        m_pUi->stackedDiskAndCloud->setCurrentIndex(1);
    }
}

void BBProjectDialog::switchFolderAndNew(QAbstractButton *pButton)
{
    if (pButton->objectName() == "buttonFolder")
    {
        m_pUi->stackedFolderAndNew->setCurrentIndex(0);
    }
    else if (pButton->objectName() == "buttonNew")
    {
        m_pUi->stackedFolderAndNew->setCurrentIndex(1);
    }
}

void BBProjectDialog::selectFolder()
{

}

void BBProjectDialog::createNewProject()
{

}

void BBProjectDialog::closeDialog()
{
    exit(0);
}

void BBProjectDialog::openSelectedProject(QListWidgetItem *pItem)
{

}

void BBProjectDialog::setButtonTabBar()
{
    QButtonGroup *pGroup = new QButtonGroup;
    pGroup->addButton(m_pUi->buttonDisk);
    pGroup->addButton(m_pUi->buttonCloud);
    QObject::connect(pGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(switchDiskAndCloud(QAbstractButton*)));

    pGroup = new QButtonGroup;
    pGroup->addButton(m_pUi->buttonFolder);
    pGroup->addButton(m_pUi->buttonNew);
    QObject::connect(pGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(switchFolderAndNew(QAbstractButton*)));
}

void BBProjectDialog::loadExistingProject()
{
    // Read the existing project on this computer
    QSettings settings(BB_USER_NAME, BB_USER_PROJECT_NAME);
    int nCount = settings.beginReadArray(m_ProjectDirArrayKey);
    m_nProjectCount = nCount;
    // clear the project list that is saved the last time
    m_ProjectDirs.clear();
    for (int i = 0; i < nCount; i++)
    {
        settings.setArrayIndex(i);
        QString projectDir = settings.value(m_ProjectDirKey).toString();
        QDir dir;
        if (dir.exists(projectDir))
        {
            // if the project exists, record it
            m_ProjectDirs.append(projectDir);
        }
        else
        {
            // user has deleted the project
            m_nProjectCount--;
        }
    }
    settings.endArray();
    // update project list
    settings.beginWriteArray(m_ProjectDirArrayKey, m_nProjectCount);
    for (int i = 0; i < m_nProjectCount; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue(m_ProjectDirKey, m_ProjectDirs.at(i));
    }
    settings.endArray();
}

void BBProjectDialog::setListWidget()
{
    QObject::connect(m_pUi->listDiskProject, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(openSelectedProject(QListWidgetItem*)));
    // icon is at the top, and text is at the bottom
    m_pUi->listDiskProject->setViewMode(QListView::IconMode);
    m_pUi->listDiskProject->setIconSize(QSize(135, 135));
    m_pUi->listDiskProject->setSpacing(0);
    // show existing projects in the list widget
    for (int i = 0; i < m_ProjectDirs.count(); i++)
    {
        QString projectDir = m_ProjectDirs.at(i);
        QString projectName = "test name";
        QString iconPath = "../../resources/icons/empty2.png";
        // Cut the overview image into a square
        QPixmap pix(iconPath);
        int h = pix.height();
        int w = pix.width();
        int size = h < w ? h : w;
        pix = pix.copy((w - size) / 2, (h - size) / 2, size, size);

        QListWidgetItem *pItem = new QListWidgetItem(m_pUi->listDiskProject);
        pItem->setIcon(QIcon(pix));
        pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        pItem->setText(projectName);
        pItem->setSizeHint(QSize(141, 160));
    }
}

void BBProjectDialog::setLineEdit()
{
    // The dir default is the last used dir
//    QSettings settings(BB_USER_NAME, BB_USER_PROJECT_NAME);
//    m_pUi->lineEditLocation->setText(settings.value("WorkSpaceDirectory").value<QString>());
//    // Give an initial project name, if it exists, increase the number
//    QDir dir;
//    int i = 1;
//    while (dir.exists(m_pUi->lineEditLocation->text() + "/New Project " + QString::number(i)))
//    {
//        i++;
//    }
//    m_pUi->lineEditProjectName->setText("New Project " + QString::number(i));
}








//void BeginningDialog::selectFolder()
//{
//    QSettings settings(MyName, MyProjectName);
//    QString directory = settings.value("workspaceDirectory").value<QString>();
//    QString fileName = QFileDialog::getExistingDirectory(this, "Select Folder", directory);
//    if (!fileName.isNull())
//    {
//        //点的确定
//        //编辑框显示工作空间地址
//        ui->lineEditLocation->setText(fileName);
//        //保存默认工作空间地址 下次显示
//        settings.setValue("workspaceDirectory", fileName);
//    }
//}

//void BeginningDialog::createNewProject()
//{
//    //创建工程文件夹
//    QDir *dir = new QDir();
//    QString directoryName = ui->lineEditLocation->text() + "/" + ui->lineEditProjectName->text();
//    bool result = dir->exists(directoryName);
//    if (result)
//    {
//        ui->lineEditProjectName->setFocus();
//        ui->lineEditProjectName->selectAll();
//    }
//    else
//    {
//        result = dir->mkdir(directoryName);
//        if (result)
//        {
//            //创建项目成功
//            //创建工程所需子文件夹
//            //用于存放用户的文件
//            dir->mkdir(directoryName + "/contents");
//            //用于存放引擎与项目相关的文件
//            dir->mkdir(directoryName + "/engine");
//            //保存工程相关的全局变量
//            projectName = ui->lineEditProjectName->text();
//            projectPath = directoryName + "/";
//            projectEngineFolderPath = projectPath + "engine/";
//            //预置一张新工程总览图在engine文件夹中
//            QFile::copy(engineResourcesPicturesPath + overviewMapName, projectEngineFolderPath + overviewMapName);
//            //保存工程路径 添加到工程路径组中 用于下次打开已有工程
//            QSettings settings(MyName, MyProjectName);
//            settings.beginWriteArray(projectDirArrayKey);
//            //添加到工程目录表尾
//            settings.setArrayIndex(projectCount);
//            settings.setValue(projectDirKey, directoryName);
//            settings.endArray();
//            //关闭对话框
//            close();
//            //显示主界面
//            mMainWidget->show();
//            //新工程的预设定
//            MainWindow *mainWindow = qobject_cast<MainWindow*>(mMainWidget->layout()->itemAt(1)->widget());
//            if (mainWindow)
//                mainWindow->newProject();
//        }
//    }
//}


//void BeginningDialog::openSelectedProject(QListWidgetItem *item)
//{
//    //打开已存在的项目
//    //获取项目路径
//    QString dirName = projectDirs.at(ui->listDiskProject->row(item));
//    //保存工程相关的全局变量
//    projectName = item->text();
//    projectPath = dirName + "/";
//    projectEngineFolderPath = projectPath + "engine/";
//    //关闭对话框
//    close();
//    //显示主界面
//    mMainWidget->show();
//    //读取已有工程 加载到主界面
//    MainWindow *mainWindow = qobject_cast<MainWindow*>(mMainWidget->layout()->itemAt(1)->widget());
//    if (mainWindow)
//        mainWindow->openExistingProject();
//}

