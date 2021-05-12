#include "BBOpenGLWidget.h"
#include <QSurfaceFormat>
#include "BBUtils.h"
#include <QTimer>
#include <QMatrix4x4>

BBOpenGLWidget::BBOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);

    // paint once per frame
    m_pRenderTimer = new QTimer(this);
    // update() perform paintGL()
    QObject::connect(m_pRenderTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_pRenderTimer->setInterval(BB_CONSTANT_UPDATE_RATE);
    m_pRenderTimer->start();
}

BBOpenGLWidget::~BBOpenGLWidget()
{
    BB_SAFE_DELETE(m_pRenderTimer);
}

void BBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // Anti-aliasing
//    glShadeModel(GL_SMOOTH);
    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    QMatrix4x4 mat;
    mat.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    glLoadMatrixf(mat.data());
    // model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Clear the color, the background becomes black, don't care the alpha
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

//    scene.init();
}

void BBOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width * devicePixelRatio(), height * devicePixelRatio());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    QMatrix4x4 mat;
    mat.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    glLoadMatrixf(mat.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    scene.resize(width, height);
}

void BBOpenGLWidget::paintGL()
{
    // Erase the current background color, color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);
//    scene.render();
}
