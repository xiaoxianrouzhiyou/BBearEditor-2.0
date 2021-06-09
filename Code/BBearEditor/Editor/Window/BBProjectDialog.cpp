#include "BBProjectDialog.h"
#include "ui_BBProjectDialog.h"
#include "BBUtils.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>


QString BBProjectDialog::m_ProjectDirArrayKey = "ProjectDirArrayKey";
QString BBProjectDialog::m_ProjectDirKey = "ProjectDirKey";
QString BBProjectDialog::m_WorkSpaceDirKey = "WorkSpaceDirKey";


BBProjectDialog::BBProjectDialog(QWidget *pParent)
    : QDialog(pParent),
      m_pUi(new Ui::BBProjectDialog)
{
    m_pUi->setupUi(this);

    QObject::connect(m_pUi->buttonClose, SIGNAL(clicked()), this, SLOT(closeDialog()));
    QObject::connect(m_pUi->buttonOpenLocation, SIGNAL(clicked()), this, SLOT(selectFolder()));
    QObject::connect(m_pUi->buttonCreate, SIGNAL(clicked()), this, SLOT(createNewProject()));

    setButtonTabBar();
    loadExistingProject();
    setListWidget();
    setLineEdit();

    m_pUi->stackedFolderAndNew->setCurrentIndex(0);
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
    QSettings settings(BB_USER_NAME, BB_USER_PROJECT_NAME);
    QString directory = settings.value(m_WorkSpaceDirKey).value<QString>();
    QString filePath = QFileDialog::getExistingDirectory(this, "Select Folder", directory);
    if (!filePath.isNull())
    {
        // The line edit shows the address of the workspace
        m_pUi->lineEditLocation->setText(filePath);
        // Save the default workspace address and show it next time
        settings.setValue(m_WorkSpaceDirKey, filePath);
    }
}

void BBProjectDialog::createNewProject()
{
    // create project folder
    QDir dir;
    QString fileName = m_pUi->lineEditProjectName->text();
    QString filePath = m_pUi->lineEditLocation->text() + "/" + fileName;
    BB_PROCESS_ERROR_RETURN(!dir.exists(filePath));

    m_pUi->lineEditProjectName->setFocus();
    m_pUi->lineEditProjectName->selectAll();

    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath));
    // Successfully created the project
    // Create subfolders required for the project
    // Used to save user's files
    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath + "/" + BBConstant::BB_NAME_FILE_SYSTEM_USER));
    // Used to save engine-related files
    BB_PROCESS_ERROR_RETURN(dir.mkdir(filePath + "/" + BBConstant::BB_NAME_FILE_SYSTEM_ENGINE));
    // save constant
    BBConstant::BB_NAME_PROJECT = fileName;
    BBConstant::BB_PATH_PROJECT = filePath + "/";
    BBConstant::BB_PATH_PROJECT_USER = BBConstant::BB_PATH_PROJECT + BBConstant::BB_NAME_FILE_SYSTEM_USER;
    BBConstant::BB_PATH_PROJECT_ENGINE = BBConstant::BB_PATH_PROJECT + BBConstant::BB_NAME_FILE_SYSTEM_ENGINE;
    // Place a project overview map in the engine folder
    BB_PROCESS_ERROR_RETURN(QFile::copy(BB_PATH_RESOURCE_PICTURE(BBConstant::BB_NAME_OVERVIEW_MAP),
                                        BBConstant::BB_PATH_PROJECT_ENGINE + "/" + BBConstant::BB_NAME_OVERVIEW_MAP));
    // save the project into QSettings
    QSettings settings(BB_USER_NAME, BB_USER_PROJECT_NAME);
    settings.beginWriteArray(m_ProjectDirArrayKey);
    // Add to the end of the project catalog
    settings.setArrayIndex(m_nProjectCount);
    settings.setValue(m_ProjectDirKey, filePath);
    settings.endArray();
    // Close the dialog
    accept();

    createProject();
}

void BBProjectDialog::closeDialog()
{
    reject();
}

void BBProjectDialog::openSelectedProject(QListWidgetItem *pItem)
{
    QString filePath = m_ProjectDirs.at(m_pUi->listDiskProject->row(pItem));
    // save constant
    BBConstant::BB_NAME_PROJECT = pItem->text();
    BBConstant::BB_PATH_PROJECT = filePath + "/";
    BBConstant::BB_PATH_PROJECT_USER = BBConstant::BB_PATH_PROJECT + BBConstant::BB_NAME_FILE_SYSTEM_USER;
    BBConstant::BB_PATH_PROJECT_ENGINE = BBConstant::BB_PATH_PROJECT + BBConstant::BB_NAME_FILE_SYSTEM_ENGINE;
    // Close the dialog
    accept();

    openProject();
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
        QFileInfo info(projectDir);
        QString projectName = info.fileName();
        QString iconPath = projectDir + "/" + BBConstant::BB_NAME_FILE_SYSTEM_ENGINE + "/" + BBConstant::BB_NAME_OVERVIEW_MAP;
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
    QSettings settings(BB_USER_NAME, BB_USER_PROJECT_NAME);
    m_pUi->lineEditLocation->setText(settings.value(m_WorkSpaceDirKey).value<QString>());
    // Give an initial project name, if it exists, increase the number
    QDir dir;
    int i = 1;
    while (dir.exists(m_pUi->lineEditLocation->text() + "/New Project " + QString::number(i)))
    {
        i++;
    }
    m_pUi->lineEditProjectName->setText("New Project " + QString::number(i));
}
