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
    Editor/PropertyManager/GroupManager/BBGroupManager.cpp \
    Editor/PropertyManager/BBPropertyFactory.cpp \
    Editor/PropertyManager/BBPropertyManager.cpp \
    Editor/Dialog/BBConfirmationDialog.cpp \
    Editor/MainInterface/BBMainWindow.cpp \
    Editor/Dialog/BBProjectDialog.cpp \
    Editor/MainInterface/BBTitleBar.cpp \
    Editor/SceneManager/BBHierarchyTreeWidget.cpp \
    Editor/ObjectList/BBObjectListWidget.cpp \
    Editor/Common/BBResizableWidget.cpp \
    Editor/Common/BBTreeWidget.cpp \
    Editor/Render/BBEditViewDockWidget.cpp \
    Editor/Render/BBEditViewOpenGLWidget.cpp \
    Editor/Render/BBOpenGLWidget.cpp \
    Editor/Common/NcFramelessHelper.cpp \
    Engine/3D/Mesh/BBMesh.cpp \
    Engine/3D/BBModel.cpp \
    Engine/3D/BBSkyBox.cpp \
    Engine/3D/Mesh/BBStaticMesh.cpp \
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
    Engine/CoordinateSystem/BBCoordinateSystem.cpp \
    Engine/3D/BBHorizontalPlane.cpp \
    Engine/2D/BBSelectionRegion.cpp \
    Engine/Utils/BBUtils.cpp \
    Engine/3D/BBLightIndicator.cpp \
    Engine/2D/BBFullScreenQuad.cpp \
    Editor/Render/BBPreviewOpenGLWidget.cpp \
    Engine/Serializer/BBMaterial.pb.cc \
    Editor/PropertyManager/BBHeadManager.cpp \
    Engine/Geometry/BBPlane.cpp \
    Engine/Geometry/BBFrustum.cpp \
    Engine/2D/BBSprite2D.cpp \
    Engine/2D/BBCanvas.cpp \
    Engine/CoordinateSystem/BBCoordinateComponent.cpp \
    Engine/CoordinateSystem/BBCoordinateSystem2D.cpp \
    Engine/Base/BBRenderableObject2D.cpp \
    Engine/CoordinateSystem/BBCoordinateComponent2D.cpp \
    Engine/CoordinateSystem/BBTransformCoordinateSystem.cpp \
    Engine/Geometry/BBBoundingBox2D.cpp \
    Engine/2D/BBSpriteObject2D.cpp \
    Engine/Scene/BBRendererManager.cpp \
    Engine/2D/BBClipArea2D.cpp \
    Engine/3D/Mesh/BBTerrain.cpp \
    Engine/Font/BBDynamicFont.cpp \
    Editor/Tools/FBX2BBear/BBFBX2BBear.cpp \
    Editor/Tools/FBX2BBear/BBFBXSkeletonGPU.cpp \
    Engine/RayTracing/BBRayTracker.cpp \
    Engine/Python/BBPythonLoader.cpp \
    Editor/Dialog/BBResourceDialog.cpp \
    Engine/Render/BBRenderQueue.cpp \
    Editor/PropertyManager/GroupManager/BBMaterialPropertyGroupManager.cpp \
    Editor/PropertyManager/GroupManager/BBLightManager.cpp \
    Engine/Serializer/BBCubeMap.pb.cc

HEADERS += \
    Editor/FileSystem/BBFileListWidget.h \
    Editor/FileSystem/BBFilePathBarWidget.h \
    Editor/FileSystem/BBFileSystemDataManager.h \
    Editor/FileSystem/BBFileSystemDockWidget.h \
    Editor/FileSystem/BBFileSystemManager.h \
    Editor/FileSystem/BBFolderTreeWidget.h \
    Editor/PropertyManager/BBFactoryComponent.h \
    Editor/PropertyManager/GroupManager/BBGroupManager.h \
    Editor/PropertyManager/BBPropertyFactory.h \
    Editor/PropertyManager/BBPropertyManager.h \
    Editor/Dialog/BBConfirmationDialog.h \
    Editor/MainInterface/BBMainWindow.h \
    Editor/Dialog/BBProjectDialog.h \
    Editor/MainInterface/BBTitleBar.h \
    Editor/SceneManager/BBHierarchyTreeWidget.h \
    Editor/ObjectList/BBObjectListWidget.h \
    Editor/Common/BBResizableWidget.h \
    Editor/Common/BBTreeWidget.h \
    Editor/Render/BBEditViewDockWidget.h \
    Editor/Render/BBEditViewOpenGLWidget.h \
    Editor/Render/BBOpenGLWidget.h \
    Editor/Common/NcFramelessHelper.h \
    Engine/3D/Mesh/BBMesh.h \
    Engine/3D/BBModel.h \
    Engine/3D/BBSkyBox.h \
    Engine/3D/Mesh/BBStaticMesh.h \
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
    Engine/CoordinateSystem/BBCoordinateSystem.h \
    Engine/3D/BBHorizontalPlane.h \
    Engine/2D/BBSelectionRegion.h \
    Engine/Utils/BBUtils.h \
    Engine/3D/BBLightIndicator.h \
    Engine/2D/BBFullScreenQuad.h \
    Editor/Render/BBPreviewOpenGLWidget.h \
    Engine/Serializer/BBMaterial.pb.h \
    Editor/PropertyManager/BBHeadManager.h \
    Engine/Geometry/BBPlane.h \
    Engine/Geometry/BBFrustum.h \
    Engine/2D/BBSprite2D.h \
    Engine/2D/BBCanvas.h \
    Engine/CoordinateSystem/BBCoordinateComponent.h \
    Engine/CoordinateSystem/BBCoordinateSystem2D.h \
    Engine/Base/BBRenderableObject2D.h \
    Engine/CoordinateSystem/BBCoordinateComponent2D.h \
    Engine/CoordinateSystem/BBTransformCoordinateSystem.h \
    Engine/Geometry/BBBoundingBox2D.h \
    Engine/2D/BBSpriteObject2D.h \
    Engine/Scene/BBRendererManager.h \
    Engine/2D/BBClipArea2D.h \
    Engine/3D/Mesh/BBTerrain.h \
    Engine/Font/BBDynamicFont.h \
    Editor/Tools/FBX2BBear/BBFBX2BBear.h \
    Editor/Tools/FBX2BBear/BBFBXSkeletonGPU.h \
    Engine/RayTracing/BBRayTracker.h \
    Engine/Python/BBPythonLoader.h \
    Editor/Dialog/BBResourceDialog.h \
    Engine/Render/BBRenderQueue.h \
    Editor/PropertyManager/GroupManager/BBMaterialPropertyGroupManager.h \
    Editor/PropertyManager/GroupManager/BBLightManager.h \
    Engine/Serializer/BBCubeMap.pb.h

FORMS += \
    Editor/FileSystem/BBFileSystemDockWidget.ui \
    Editor/Dialog/BBConfirmationDialog.ui \
    Editor/MainInterface/BBMainWindow.ui \
    Editor/Dialog/BBProjectDialog.ui \
    Editor/MainInterface/BBTitleBar.ui \
    Editor/Dialog/BBResourceDialog.ui

INCLUDEPATH += \
    ../BBearEditor/Editor \
    ../BBearEditor/Editor/Common \
    ../BBearEditor/Engine \
    ../../External \
    ../../External/Eigen \
    ../../External/rapidxml \
    ../../External/FBX/include \
    ../../External/lua \
    ../../External/ProtoBuffer/src

LIBS += -lopengl32 -lglu32 -lglut \
    -L../../External/ProtoBuffer/lib -lprotobuf \

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../External/Python37-32/libs/ -lpython37
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../External/Python37-32/libs/ -lpython37d

INCLUDEPATH += $$PWD/../../External/Python37-32/include
DEPENDPATH += $$PWD/../../External/Python37-32/include
