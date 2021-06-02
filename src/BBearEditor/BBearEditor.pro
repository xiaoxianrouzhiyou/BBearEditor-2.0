#-------------------------------------------------
#
# Project created by QtCreator 2021-05-09T15:51:16
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BBearEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    NcFramelessHelper.cpp \
    BBObjectListWidget.cpp \
    BBUtils.cpp \
    BBResizableWidget.cpp \
    BBTreeWidget.cpp \
    BBHierarchyTreeWidget.cpp \
    BBGameObject.cpp \
    Window/BBConfirmationDialog.cpp \
    BBOpenGLWidget.cpp \
    BBScene.cpp \
    BBCamera.cpp \
    BBEditViewDockWidget.cpp \
    BBEditViewOpenGLWidget.cpp \
    BBRenderableObject.cpp \
    BBGLShader.cpp \
    BBGLBuffers.cpp \
    BBSkyBox.cpp \
    BBHorizontalPlane.cpp \
    BBMesh.cpp \
    BBModel.cpp \
    BBRay.cpp \
    BBBoundingBox.cpp \
    BBSelectionRegion.cpp \
    BBCoordinateSystem.cpp \
    BBStaticMesh.cpp \
    BBGameObjectSet.cpp \
    Window/BBTitleBar.cpp \
    Window/BBMainWindow.cpp \
    PropertyManager/BBPropertyManager.cpp \
    PropertyManager/BBGroupManager.cpp \
    PropertyManager/BBPropertyFactory.cpp \
    PropertyManager/BBFactoryComponent.cpp \
    FileSystem/BBFolderTreeWidget.cpp \
    Window/BBProjectDialog.cpp \
    FileSystem/BBFileListWidget.cpp \
    FileSystem/BBFilePathBarWidget.cpp \
    FileSystem/BBFileSystemDockWidget.cpp \
    FileSystem/BBFileSystemDataManager.cpp \
    FileSystem/BBFileSystemManager.cpp

HEADERS += \
    NcFramelessHelper.h \
    BBObjectListWidget.h \
    BBUtils.h \
    BBResizableWidget.h \
    BBTreeWidget.h \
    BBHierarchyTreeWidget.h \
    BBGameObject.h \
    Window/BBConfirmationDialog.h \
    BBOpenGLWidget.h \
    BBScene.h \
    BBCamera.h \
    BBEditViewDockWidget.h \
    BBEditViewOpenGLWidget.h \
    BBRenderableObject.h \
    BBGLShader.h \
    BBGLBuffers.h \
    BBSkyBox.h \
    BBHorizontalPlane.h \
    BBMesh.h \
    BBModel.h \
    BBRay.h \
    BBBoundingBox.h \
    BBSelectionRegion.h \
    BBCoordinateSystem.h \
    BBStaticMesh.h \
    BBGameObjectSet.h \
    Window/BBTitleBar.h \
    Window/BBMainWindow.h \
    PropertyManager/BBPropertyManager.h \
    PropertyManager/BBGroupManager.h \
    PropertyManager/BBPropertyFactory.h \
    PropertyManager/BBFactoryComponent.h \
    FileSystem/BBFolderTreeWidget.h \
    Window/BBProjectDialog.h \
    FileSystem/BBFileListWidget.h \
    FileSystem/BBFilePathBarWidget.h \
    FileSystem/BBFileSystemDockWidget.h \
    FileSystem/BBFileSystemDataManager.h \
    FileSystem/BBFileSystemManager.h

FORMS += \
    Window/BBConfirmationDialog.ui \
    Window/BBMainWindow.ui \
    Window/BBTitleBar.ui \
    Window/BBProjectDialog.ui \
    FileSystem/BBFileSystemDockWidget.ui

RESOURCES += \
    ../../resources/BBBasicResource.qrc

INCLUDEPATH += \
    ../../includes \
    ../../includes/Eigen \
    ../../includes/rapidxml \
    ../../includes/fbxsdk \
    ../../includes/lua \

LIBS += -lopengl32 -lglu32 -lglut
