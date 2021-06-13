#include "BBEditViewDockWidget.h"
#include <QKeyEvent>
#include "Scene/BBSceneManager.h"
#include <QFileDialog>
#include "FileSystem/BBFileSystemDataManager.h"
#include <QVBoxLayout>
#include <QLabel>


BBEditViewDockWidget::BBEditViewDockWidget(QWidget *pParent)
    : QDockWidget(pParent)
{
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBar();
}

void BBEditViewDockWidget::setTitleBarText()
{
    QString fileName = BBFileSystemDataManager::getFileNameByPath(BBSceneManager::getCurrentSceneFilePath());
    QString text = "Edit View [" + BBFileSystemDataManager::getBaseName(fileName);
    if (BBSceneManager::isSceneChanged())
    {
        text += "*";
    }
    text += "]";
    m_pWindowTitle->setText(text);
}

void BBEditViewDockWidget::setTitleBar()
{
    QWidget *pBar = titleBarWidget();
    BB_SAFE_DELETE(pBar);

    pBar = new QWidget(this);
    pBar->setStyleSheet("background-color: #191f28; padding-top: 8px; padding-bottom: 8px; padding-left: 5px;");

    QVBoxLayout *pLayout = new QVBoxLayout(pBar);
    pLayout->setMargin(0);

    m_pWindowTitle = new QLabel(pBar);
    m_pWindowTitle->setStyleSheet("color: #d6dfeb; font: 75 9pt \"Arial\";");
    pLayout->addWidget(m_pWindowTitle);
    setTitleBarText();

    setTitleBarWidget(pBar);
}

void BBEditViewDockWidget::keyPressEvent(QKeyEvent *e)
{
    keyPress(e);

    if (e->isAutoRepeat())
        return;

    switch (e->key()) {
    case Qt::Key_W:
        pressMoveKey('W');
        pressTransform('W');
        break;
    case Qt::Key_A:
        pressMoveKey('A');
        break;
    case Qt::Key_S:
        pressMoveKey('S');
        break;
    case Qt::Key_D:
        pressMoveKey('D');
        break;
    case Qt::Key_Q:
        pressMoveKey('Q');
        break;
    case Qt::Key_E:
        pressMoveKey('E');
        pressTransform('E');
        break;
    case Qt::Key_R:
        pressTransform('R');
        break;
    case Qt::Key_Escape:
        pressESC();
        break;
    default:
        break;
    }

    if ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_S))
    {
        emit saveCurrentScene();
    }
}

void BBEditViewDockWidget::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_W:
        releaseMoveKey('W');
        break;
    case Qt::Key_A:
        releaseMoveKey('A');
        break;
    case Qt::Key_S:
        releaseMoveKey('S');
        break;
    case Qt::Key_D:
        releaseMoveKey('D');
        break;
    case Qt::Key_Q:
        releaseMoveKey('Q');
        break;
    case Qt::Key_E:
        releaseMoveKey('E');
        break;
    default:
        break;
    }
}

void BBEditViewDockWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    cancelFileListSelectedItems();
}

