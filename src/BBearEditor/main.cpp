#include <QApplication>
#include "NcFramelessHelper.h"
#include <QVBoxLayout>
#include "Window/BBTitleBar.h"
#include "Window/BBMainWindow.h"
#include "Window/BBProjectDialog.h"


//#include "boundingbox.h"
#include <QDesktopWidget>
//#include "assetmanager.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // main interface
    QWidget mainWidget;
//    mainWidget.setWindowState(Qt::WindowMaximized);
    mainWidget.resize(1600, 1000);
    mainWidget.setWindowFlags(Qt::FramelessWindowHint | mainWidget.windowFlags());

    NcFramelessHelper mainWidgetHelper;
    mainWidgetHelper.activateOn(&mainWidget);

    QVBoxLayout mainVlayout(&mainWidget);
    mainVlayout.setMargin(0);
    mainVlayout.setSpacing(0);

    // parent object must be passed in
    // and invoke the close function of the parent object to close the window
    // to prevent errors
    BBTitleBar titleBar(&mainWidget);
    mainVlayout.addWidget(&titleBar);

    BBMainWindow mainWindow;
    mainVlayout.addWidget(&mainWindow);

//    mainWidget.hide();
    mainWidget.show();

    // BBProjectDialog, select project to open
    BBProjectDialog dialog;
    dialog.resize(QSize(800, 480));
    dialog.setWindowFlags(Qt::FramelessWindowHint | dialog.windowFlags());
    NcFramelessHelper dialogHelper;
    dialogHelper.activateOn(&dialog);
    dialog.move((QApplication::desktop()->width()  - dialog.sizeHint().width())  / 2,
                (QApplication::desktop()->height() - dialog.sizeHint().height()) / 2);
    dialog.show();






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
