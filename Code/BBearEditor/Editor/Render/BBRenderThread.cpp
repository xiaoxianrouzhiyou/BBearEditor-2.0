#include "BBRenderThread.h"
#include <QSurfaceFormat>
#include <QTimer>
#include "BBUtils.h"


BBRenderThread::BBRenderThread(QObject *pParent)
    : QObject(pParent)
{
    // render once per frame
    m_pRenderTimer = new QTimer(this);
    QObject::connect(m_pRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    m_pRenderTimer->setInterval(BB_CONSTANT_UPDATE_RATE);
    m_pRenderTimer->start();
}

BBRenderThread::~BBRenderThread()
{
    BB_SAFE_DELETE(m_pRenderTimer);
}

void BBRenderThread::render()
{
    emit updateOpenGLWidget();
}
