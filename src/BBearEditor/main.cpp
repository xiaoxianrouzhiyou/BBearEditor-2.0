#include <QApplication>
#include "NcFramelessHelper.h"
#include <QVBoxLayout>
#include "BBUITitleBar.h"
#include "BBUIMainWindow.h"


//#include "boundingbox.h"
//#include "beginningdialog.h"
#include <QDesktopWidget>
//#include "assetmanager.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // main interface
    QWidget mainWidget;
    //mainWidget.setWindowState(Qt::WindowMaximized);
    mainWidget.resize(1600, 1000);
    mainWidget.setWindowFlags(Qt::FramelessWindowHint | mainWidget.windowFlags());

    NcFramelessHelper fHelper;
    fHelper.activateOn(&mainWidget);

    QVBoxLayout mainVlayout(&mainWidget);
    mainVlayout.setMargin(0);
    mainVlayout.setSpacing(0);

    // parent object must be passed in
    // and invoke the close function of the parent object to close the window
    // to prevent errors
    BBUITitleBar titleBar(&mainWidget);
    mainVlayout.addWidget(&titleBar);

    BBUIMainWindow mainWindow;
    mainVlayout.addWidget(&mainWindow);

    //mainWidget.hide();
    mainWidget.show();






    //开始对话框
    //BeginningDialog beginningDialog(&mainWidget);
    //beginningDialog.resize(QSize(800, 480));
    //beginningDialog.setWindowFlags(Qt::FramelessWindowHint | beginningDialog.windowFlags());
    //NcFramelessHelper *beginningFh = new NcFramelessHelper;
    //beginningFh->activateOn(&beginningDialog);
    //beginningDialog.move((QApplication::desktop()->width() - beginningDialog.sizeHint().width()) / 2,
    //            (QApplication::desktop()->height() - beginningDialog.sizeHint().height()) / 2);
    //beginningDialog.show();

    //支持R屏
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    //不用mac的风格
    a.setStyle(QStyleFactory::create("Fusion"));

    //绑定全局信号槽
    //QObject::connect(&a, SIGNAL(pressTransformSignal(char)), &mainWindow, SLOT(globalPressTransformKey(char)));
    //QObject::connect(&a, SIGNAL(multipleSelectKey(bool)), &mainWindow, SLOT(multipleSelectKey(bool)));
    //QObject::connect(&titleBar, SIGNAL(run(bool)), &mainWindow, SLOT(run(bool)));

    return a.exec();
}
