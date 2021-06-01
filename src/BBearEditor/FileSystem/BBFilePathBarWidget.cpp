#include "BBFilePathBarWidget.h"
#include "BBUtils.h"
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>


//---------------------------------------------------------------------------------------------------
//  BBFilePathBarScrollArea
//---------------------------------------------------------------------------------------------------

BBFilePathBarScrollArea::BBFilePathBarScrollArea(QWidget *pParent)
    : QScrollArea(pParent)
{
    QObject::connect(horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(moveToEnd()));
}

void BBFilePathBarScrollArea::moveToEnd()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->maximumWidth());
}

void BBFilePathBarScrollArea::moveToLeft()
{
    scroll(-30);
}

void BBFilePathBarScrollArea::moveToRight()
{
    scroll(30);
}

void BBFilePathBarScrollArea::scroll(int nStep)
{
    horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->sliderPosition() + nStep);
}


//---------------------------------------------------------------------------------------------------
//  BBFilePathBarWidget
//---------------------------------------------------------------------------------------------------

BBFilePathBarWidget::BBFilePathBarWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(3);
    setStyleSheet("QWidget {border: none; color: #d6dfeb; font: 9pt \"Arial\";}"
                  "QPushButton {background: none; border-radius: 2px; padding: 1px 3px;}"
                  "QPushButton:hover {background: #0ebf9c;}"
                  "QLabel {color: rgb(60, 64, 75);}");
}

void BBFilePathBarWidget::update()
{
    showFolderPath(m_CurrentPath);
}

void BBFilePathBarWidget::showFolderPath(const QString &path)
{
    m_CurrentPath = path;
    // Clear the last showed path
    while (QLayoutItem *pChild = layout()->takeAt(0))
    {
        delete pChild->widget();
    }
    // The relative path that is relative to project directory
    QString relativePath = path.mid(BBConstant::BB_PATH_PROJECT.length());
    m_HierarchyDirs = relativePath.split('/');

    QButtonGroup *pGroup = new QButtonGroup;
    // do not need label separator before the first item
    QPushButton *pButton = new QPushButton(m_HierarchyDirs.at(0));
    pGroup->addButton(pButton, 0);
    layout()->addWidget(pButton);
    // add items after the first item
    for (int i = 1; i < m_HierarchyDirs.count(); i++)
    {
        QLabel *pLabel = new QLabel("»");
        layout()->addWidget(pLabel);
        pButton = new QPushButton(m_HierarchyDirs.at(i));
        pGroup->addButton(pButton, i);
        layout()->addWidget(pButton);
    }

    QObject::connect(pGroup, SIGNAL(buttonClicked(int)), this, SLOT(accessFolder(int)));
}

void BBFilePathBarWidget::accessFolder(int id)
{
    QString path = BBConstant::BB_PATH_PROJECT;
    for (int i = 0; i < id; i++)
    {
        path += m_HierarchyDirs.at(i) + "/";
    }
    path += m_HierarchyDirs.at(id);
    accessFolder(path);
}
