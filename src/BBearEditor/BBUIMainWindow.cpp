#include "BBUIMainWindow.h"
#include "ui_BBUIMainWindow.h"
#include "BBUtils.h"

BBUIMainWindow::BBUIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::BBUIMainWindow)
{
    m_pUi->setupUi(this);
    setWindowLayout();
    setGameObjectDockWidget();
    setConnect();
}

BBUIMainWindow::~BBUIMainWindow()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBUIMainWindow::setWindowLayout()
{
    // Allow nesting of docks
    setDockNestingEnabled(true);

    // dock layout
    addDockWidget(Qt::LeftDockWidgetArea, m_pUi->dockGameObject);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockEditview, Qt::Horizontal);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockInspector, Qt::Horizontal);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockHierarchy, Qt::Vertical);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockProject, Qt::Vertical);
//    tabifyDockWidget(m_pUi->dockPreview, m_pUi->dockEditview);
//    tabifyDockWidget(m_pUi->dockConsole, m_pUi->dockProject);
//    splitDockWidget(m_pUi->dockProject, m_pUi->dockConsole, Qt::Horizontal);
//    m_pUi->dockPreview->setVisible(false);
//    m_pUi->dockConsole->setVisible(false);

    // resize
    m_pUi->dockGameObjectContents->updateSizeHint(QSize(350, 150));
    m_pUi->dockInspectorContents->updateSizeHint(QSize(300, 500));
    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));

    // Stretch the width of the file list in the project view
    m_pUi->splitterProject->setStretchFactor(1, 1);
}

void BBUIMainWindow::setGameObjectDockWidget()
{
    // Load contents for list
    m_pUi->listBaseGameObject->loadListItems(BB_PATH_BASEOBJECTLIST);
    m_pUi->listBaseGameObject->setMimeType(BB_MIMETYPE_BASEOBJECT);
}

void BBUIMainWindow::setConnect()
{
    // The handling of key events in the EditView in OpenGL
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressMoveKeySignal(char)),
                     m_pUi->openGLWidget, SLOT(pressMoveKeySlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(releaseMoveKeySignal(char)),
                     m_pUi->openGLWidget, SLOT(releaseMoveKeySlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressTransformSignal(char)),
                     m_pUi->openGLWidget, SLOT(pressTransformSlot(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressESCSignal()), m_pUi->openGLWidget, SLOT(pressESCSlot()));
    QObject::connect(m_pUi->dockEditview, SIGNAL(keyPress(QKeyEvent*)), m_pUi->openGLWidget, SLOT(onKeyPress(QKeyEvent*)));
//    QObject::connect(this, SIGNAL(sceneMultipleSelectKey(bool)), ui->openGLWidget, SLOT(multipleSelectKey(bool)));
}
