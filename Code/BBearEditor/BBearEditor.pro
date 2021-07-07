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
    Editor/FileSystem/BBFileListWidget.cpp \
    Editor/FileSystem/BBFilePathBarWidget.cpp \
    Editor/FileSystem/BBFileSystemDataManager.cpp \
    Editor/FileSystem/BBFileSystemDockWidget.cpp \
    Editor/FileSystem/BBFileSystemManager.cpp \
    Editor/FileSystem/BBFolderTreeWidget.cpp \
    Editor/PropertyManager/BBFactoryComponent.cpp \
    Editor/PropertyManager/BBGroupManager.cpp \
    Editor/PropertyManager/BBPropertyFactory.cpp \
    Editor/PropertyManager/BBPropertyManager.cpp \
    Editor/Window/BBConfirmationDialog.cpp \
    Editor/Window/BBMainWindow.cpp \
    Editor/Window/BBProjectDialog.cpp \
    Editor/Window/BBTitleBar.cpp \
    Editor/SceneManager/BBHierarchyTreeWidget.cpp \
    Editor/Common/BBObjectListWidget.cpp \
    Editor/Common/BBResizableWidget.cpp \
    Editor/Common/BBTreeWidget.cpp \
    Editor/Render/BBEditViewDockWidget.cpp \
    Editor/Render/BBEditViewOpenGLWidget.cpp \
    Editor/Render/BBOpenGLWidget.cpp \
    Editor/Common/NcFramelessHelper.cpp \
    Engine/3D/BBMesh.cpp \
    Engine/3D/BBModel.cpp \
    Engine/3D/BBSkyBox.cpp \
    Engine/3D/BBStaticMesh.cpp \
    Engine/Base/BBGameObject.cpp \
    Engine/Base/BBGameObjectSet.cpp \
    Engine/Base/BBRenderableObject.cpp \
    Engine/Geometry/BBBoundingBox.cpp \
    Engine/Geometry/BBRay.cpp \
    Engine/Render/BBGLBuffers.cpp \
    Engine/Render/BBCamera.cpp \
    Engine/Scene/BBScene.cpp \
    Engine/Allocator/tlsf/tlsf.c \
    Engine/Allocator/BBAllocator.cpp \
    Engine/Profiler/BBProfiler.cpp \
    Engine/Scene/BBSceneManager.cpp \
    Engine/Serializer/BBGameObject.pb.cc \
    Engine/Serializer/BBHierarchyTreeWidgetItem.pb.cc \
    Engine/Serializer/BBVector.pb.cc \
    Engine/Serializer/BBScene.pb.cc \
    Engine/Render/BBVertexBufferObject.cpp \
    Engine/Render/BBElementBufferObject.cpp \
    Engine/Render/BBRenderState.cpp \
    Engine/Render/BBMaterial.cpp \
    Engine/Render/BBBufferObject.cpp \
    Engine/Render/BBAttribute.cpp \
    Engine/Render/BBBaseRenderComponent.cpp \
    Engine/Render/BBLinkedList.cpp \
    Engine/Render/BBShader.cpp \
    Engine/Render/BBUniformUpdater.cpp \
    Engine/Render/BBDrawCall.cpp \
    Engine/Render/BBMaterialProperty.cpp \
    Engine/Render/BBRenderPass.cpp \
    Engine/Render/Light/BBLight.cpp \
    Engine/Render/BBFrameBufferObject.cpp \
    Engine/Render/BBTexture.cpp \
    Engine/Render/Light/BBDirectionalLight.cpp \
    Engine/Render/Light/BBPointLight.cpp \
    Engine/Render/Light/BBSpotLight.cpp \
    Engine/3D/BBIcon.cpp \
    Engine/3D/BBCoordinateSystem.cpp \
    Engine/3D/BBHorizontalPlane.cpp \
    Engine/2D/BBSelectionRegion.cpp \
    Engine/Utils/BBUtils.cpp \
    Engine/3D/BBLightIndicator.cpp \
    Engine/2D/BBFullScreenQuad.cpp \
    Editor/Render/BBPreviewOpenGLWidget.cpp \
    Engine/Serializer/BBMaterial.pb.cc \
    Engine/IO/BBMaterialFileManager.cpp \
    Editor/PropertyManager/BBHeadManager.cpp \
    Engine/Geometry/BBPlane.cpp \
    Engine/Geometry/BBFrustum.cpp

HEADERS += \
    Editor/FileSystem/BBFileListWidget.h \
    Editor/FileSystem/BBFilePathBarWidget.h \
    Editor/FileSystem/BBFileSystemDataManager.h \
    Editor/FileSystem/BBFileSystemDockWidget.h \
    Editor/FileSystem/BBFileSystemManager.h \
    Editor/FileSystem/BBFolderTreeWidget.h \
    Editor/PropertyManager/BBFactoryComponent.h \
    Editor/PropertyManager/BBGroupManager.h \
    Editor/PropertyManager/BBPropertyFactory.h \
    Editor/PropertyManager/BBPropertyManager.h \
    Editor/Window/BBConfirmationDialog.h \
    Editor/Window/BBMainWindow.h \
    Editor/Window/BBProjectDialog.h \
    Editor/Window/BBTitleBar.h \
    Editor/SceneManager/BBHierarchyTreeWidget.h \
    Editor/Common/BBObjectListWidget.h \
    Editor/Common/BBResizableWidget.h \
    Editor/Common/BBTreeWidget.h \
    Editor/Render/BBEditViewDockWidget.h \
    Editor/Render/BBEditViewOpenGLWidget.h \
    Editor/Render/BBOpenGLWidget.h \
    Editor/Common/NcFramelessHelper.h \
    Engine/3D/BBMesh.h \
    Engine/3D/BBModel.h \
    Engine/3D/BBSkyBox.h \
    Engine/3D/BBStaticMesh.h \
    Engine/Base/BBGameObject.h \
    Engine/Base/BBGameObjectSet.h \
    Engine/Base/BBRenderableObject.h \
    Engine/Geometry/BBBoundingBox.h \
    Engine/Geometry/BBRay.h \
    Engine/Render/BBGLBuffers.h \
    Engine/Render/BBCamera.h \
    Engine/Scene/BBScene.h \
    Engine/Allocator/tlsf/tlsf.h \
    Engine/Allocator/tlsf/tlsfbits.h \
    Engine/Allocator/BBAllocator.h \
    Engine/Allocator/BBMemoryLabel.h \
    Engine/Profiler/BBProfiler.h \
    Engine/Scene/BBSceneManager.h \
    Engine/Serializer/BBGameObject.pb.h \
    Engine/Serializer/BBHierarchyTreeWidgetItem.pb.h \
    Engine/Serializer/BBVector.pb.h \
    Engine/Serializer/BBScene.pb.h \
    Engine/Render/BBVertexBufferObject.h \
    Engine/Render/BBElementBufferObject.h \
    Engine/Render/BBRenderState.h \
    Engine/Render/BBMaterial.h \
    Engine/Render/BBBufferObject.h \
    Engine/Render/BBAttribute.h \
    Engine/Render/BBBaseRenderComponent.h \
    Engine/Render/BBLinkedList.h \
    Engine/Render/BBShader.h \
    Engine/Render/BBUniformUpdater.h \
    Engine/Render/BBDrawCall.h \
    Engine/Render/BBMaterialProperty.h \
    Engine/Render/BBRenderPass.h \
    Engine/Render/Light/BBLight.h \
    Engine/Render/BBFrameBufferObject.h \
    Engine/Render/BBTexture.h \
    Engine/Render/Light/BBDirectionalLight.h \
    Engine/Render/Light/BBPointLight.h \
    Engine/Render/Light/BBSpotLight.h \
    Engine/3D/BBIcon.h \
    Engine/3D/BBCoordinateSystem.h \
    Engine/3D/BBHorizontalPlane.h \
    Engine/2D/BBSelectionRegion.h \
    Engine/Utils/BBUtils.h \
    Engine/3D/BBLightIndicator.h \
    Engine/2D/BBFullScreenQuad.h \
    Editor/Render/BBPreviewOpenGLWidget.h \
    Engine/Serializer/BBMaterial.pb.h \
    Engine/IO/BBMaterialFileManager.h \
    Editor/PropertyManager/BBHeadManager.h \
    Engine/Geometry/BBPlane.h \
    Engine/Geometry/BBFrustum.h

FORMS += \
    Editor/FileSystem/BBFileSystemDockWidget.ui \
    Editor/Window/BBConfirmationDialog.ui \
    Editor/Window/BBMainWindow.ui \
    Editor/Window/BBProjectDialog.ui \
    Editor/Window/BBTitleBar.ui

RESOURCES += \
    ../../resources/BBBasicResource.qrc

INCLUDEPATH += \
    ../BBearEditor/Editor \
    ../BBearEditor/Editor/Common \
    ../BBearEditor/Engine \
    ../../External \
    ../../External/Eigen \
    ../../External/rapidxml \
    ../../External/fbxsdk \
    ../../External/lua \
    ../../External/ProtoBuffer/src \

LIBS += -lopengl32 -lglu32 -lglut \
    -L../../External/ProtoBuffer/lib -lprotobuf
