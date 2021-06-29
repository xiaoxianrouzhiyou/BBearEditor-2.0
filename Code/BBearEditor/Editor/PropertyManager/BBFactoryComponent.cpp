#include "BBFactoryComponent.h"
#include "Utils/BBUtils.h"
#include <QPixmap>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QMimeData>
#include <QFileInfo>


/**
 * @brief BBSliderLabel::BBSliderLabel
 * @param pParent
 */
BBSliderLabel::BBSliderLabel(QWidget *pParent)
    : QPushButton(pParent)
{
    m_bPressed = false;
    setMouseTracking(true);
}

void BBSliderLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPixmap pix(BB_PATH_RESOURCE_ICON(arrows_stretch_horizontal.png));
    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(32 * devicePixelRatio(), 32 * devicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setCursor(QCursor(pix));

    if (m_bPressed)
    {
        QPoint currentPos = event->globalPos();
        int nDeltaX = currentPos.x() - m_LastPos.x();
        m_LastPos = currentPos;
        if (nDeltaX != 0)
        {
            slide(nDeltaX);
        }
    }
}

void BBSliderLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_LastPos = event->globalPos();
    }
}

void BBSliderLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = false;
        m_LastPos = QPoint(0, 0);
    }
}


/**
 * @brief BBColorButton::BBColorButton
 * @param pParent
 */
BBColorButton::BBColorButton(QWidget *pParent)
    : QPushButton(pParent)
{
    // white main background
    setStyleSheet("border: none; border-radius: 2px; padding: 2px 4px; background: white;");
    // add real color widget into white background for observing transparent color
    m_pWhiteBackGroundContent = new QWidget(this);
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(1);
    pLayout->addWidget(m_pWhiteBackGroundContent);
    QHBoxLayout *pWhiteLayout = new QHBoxLayout(m_pWhiteBackGroundContent);
    pWhiteLayout->setMargin(2);
    pWhiteLayout->setSpacing(0);
    // black background
    QWidget *pBlackBackGround = new QWidget(this);
    pBlackBackGround->setStyleSheet("background: black;");
    pWhiteLayout->addWidget(pBlackBackGround);
    // add another black background for observing transparent color
    m_pBlackBackGroundContent = new QWidget(pBlackBackGround);
    QHBoxLayout *pBlackLayout = new QHBoxLayout(pBlackBackGround);
    pBlackLayout->setMargin(1);
    pBlackLayout->addWidget(m_pBlackBackGroundContent);
    // placeholder
    QWidget *pTransparent = new QWidget(m_pWhiteBackGroundContent);
    pTransparent->setStyleSheet("background: transparent;");
    pWhiteLayout->addWidget(pTransparent);
    // default
    setColor(255, 255, 255, 255);
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(click()));
}

BBColorButton::~BBColorButton()
{
    BB_SAFE_DELETE(m_pBlackBackGroundContent);
    BB_SAFE_DELETE(m_pWhiteBackGroundContent);
}

void BBColorButton::setColor(int r, int g, int b, int a)
{
    QString styleSheet = "background: rgba(" + QString::number(r) + ", " + QString::number(g) + ", "
            + QString::number(b) + ", " + QString::number(a) + ");";

    m_pBlackBackGroundContent->setStyleSheet(styleSheet);
    m_pWhiteBackGroundContent->setStyleSheet(styleSheet);
}

void BBColorButton::setColor(float *color)
{
    setColor(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);
}

void BBColorButton::click()
{
    qDebug() << "BBColorButton::click()";
}


/**
 * @brief BBScreenDialog::BBScreenDialog
 * @param pParent
 */

int BBScreenDialog::m_nCursorSize = 19;

BBScreenDialog::BBScreenDialog(QWidget *pParent)
    : QDialog(pParent)
{
    // frame
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowState(Qt::WindowMaximized);
    // Capture the screen and set the background of the dialog to be consistent with the screen
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    m_pBackground = new QLabel(this);
    pLayout->addWidget(m_pBackground);
    // Capture the screen
    setBackground();
    // Remove pop-up animation
    // hide();
    // show();
    // Set cursor as dropper
    QPixmap pix(BB_PATH_RESOURCE_ICON(eyedropper.png));

    pix.setDevicePixelRatio(devicePixelRatio());
    pix = pix.scaled(m_nCursorSize * devicePixelRatio(), m_nCursorSize * devicePixelRatio(),
                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setCursor(QCursor(pix));
}

BBScreenDialog::~BBScreenDialog()
{
    BB_SAFE_DELETE(m_pBackground);
}

void BBScreenDialog::setBackground()
{
    // Capture the screen
    m_PixBackground = QApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
    m_PixBackground.setDevicePixelRatio(devicePixelRatio());

#if defined(Q_OS_WIN32)
    // remove task bar that is in the bottom
    int y = QApplication::desktop()->availableGeometry().height() - QApplication::desktop()->screenGeometry().height();
    m_PixBackground = m_PixBackground.copy(0, y * devicePixelRatio(), m_PixBackground.width(), m_PixBackground.height());

#elif defined(Q_OS_MAC)
    // if it is mac, need to remove menu bar that is in the top
    m_PixBackground = m_PixBackground.copy(0, QApplication::desktop()->availableGeometry().y() * devicePixelRatio(),
                                           m_PixBackground.width(), m_PixBackground.height());
#endif
    m_pBackground->setPixmap(m_PixBackground);
}

void BBScreenDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QColor color = m_PixBackground.toImage().pixelColor((event->pos() + m_nCursorSize / 2 * QPoint(-1, 1)) * devicePixelRatio());
        // Set the color of the color button as the currently selected value
        emit setColor(color.redF(), color.greenF(), color.blueF());
        accept();
    }
    else
    {
        reject();
    }
}


/**
 * @brief BBPictureLabel::BBPictureLabel
 * @param pParent
 */
QSize BBPictureLabel::m_DefaultSize = QSize(60, 60);
QSize BBPictureLabel::m_ContentDefaultSize = QSize(58, 58);

BBPictureLabel::BBPictureLabel(QWidget *pParent)
    : QLabel(pParent)
{
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    setFocusPolicy(Qt::NoFocus);
    setText("None");
    setMinimumSize(m_DefaultSize * devicePixelRatio());
    setMaximumSize(m_DefaultSize * devicePixelRatio());
    setStyleSheet("color: #d6dfeb; font: 9pt \"Arial\"; border-radius: 2px;");
}

void BBPictureLabel::setScaledPixmap(const QPixmap &pixmap)
{
    setPixmap(pixmap.scaled(m_ContentDefaultSize * devicePixelRatio(), Qt::KeepAspectRatio));
}

void BBPictureLabel::dragEnterEvent(QDragEnterEvent *event)
{
    QByteArray data;
    if ((data = event->mimeData()->data(BB_MIMETYPE_FILELISTWIDGET)) != nullptr)
    {
        QDataStream dataStream(&data, QIODevice::ReadOnly);
        dataStream >> m_CurrentFilePath;
        QString suffix = QFileInfo(m_CurrentFilePath).suffix();
        if (m_Filter.contains(suffix))
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
}

void BBPictureLabel::dropEvent(QDropEvent *event)
{
    emit currentFilePathChanged(m_CurrentFilePath);
    event->accept();
}
