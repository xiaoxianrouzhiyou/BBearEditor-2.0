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
    BBUITitleBar.cpp \
    BBUIMainWindow.cpp \
    BBObjectListWidget.cpp \
    BBUtils.cpp \
    BBResizableWidget.cpp \
    BBTreeWidget.cpp \
    BBHierarchyTreeWidget.cpp \
    BBGameObject.cpp \
    BBConfirmationDialog.cpp \
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
    BBOBJMesh.cpp \
    BBModel.cpp \
    BBRay.cpp \
    BBBoundingBox.cpp \
    BBSelectionRegion.cpp

HEADERS += \
    NcFramelessHelper.h \
    BBUITitleBar.h \
    BBUIMainWindow.h \
    BBObjectListWidget.h \
    BBUtils.h \
    BBResizableWidget.h \
    BBTreeWidget.h \
    BBHierarchyTreeWidget.h \
    BBGameObject.h \
    BBConfirmationDialog.h \
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
    BBOBJMesh.h \
    BBModel.h \
    BBRay.h \
    BBBoundingBox.h \
    BBSelectionRegion.h

FORMS += \
    BBUITitleBar.ui \
    BBUIMainWindow.ui \
    BBConfirmationDialog.ui

RESOURCES += \
    BBBasicResource.qrc

INCLUDEPATH += \
    ../../includes \
    ../../includes/Eigen \
    ../../includes/rapidxml \
    ../../includes/fbxsdk \
    ../../includes/lua \

LIBS += -lopengl32 -lglu32 -lglut

DISTFILES += \
    resources/shaders/base.frag \
    resources/meshes/ashe/ashe@attack1.fbx \
    resources/meshes/ashe/ashe@attack2.fbx \
    resources/meshes/ashe/ashe@channel.fbx \
    resources/meshes/ashe/ashe@crit.fbx \
    resources/meshes/ashe/ashe@dance.fbx \
    resources/meshes/ashe/ashe@death.fbx \
    resources/meshes/ashe/ashe@idle1.fbx \
    resources/meshes/ashe/ashe@idle2.fbx \
    resources/meshes/ashe/ashe@idle3.fbx \
    resources/meshes/ashe/ashe@idle4.fbx \
    resources/meshes/ashe/ashe@idle5.fbx \
    resources/meshes/ashe/ashe@joke.fbx \
    resources/meshes/ashe/ashe@laugh.fbx \
    resources/meshes/ashe/ashe@run.fbx \
    resources/meshes/ashe/ashe@runjoge.fbx \
    resources/meshes/ashe/ashe@runwalk.fbx \
    resources/meshes/ashe/ashe@spell1.fbx \
    resources/meshes/ashe/ashe@spell2.fbx \
    resources/meshes/ashe/ashe@taunt.fbx \
    resources/meshes/ashe/ashe@windup.fbx \
    resources/skyboxs/1/Back.bmp \
    resources/skyboxs/1/bottom.bmp \
    resources/skyboxs/1/Front.bmp \
    resources/skyboxs/1/left.bmp \
    resources/skyboxs/1/right.bmp \
    resources/skyboxs/1/top.bmp \
    resources/skyboxs/2/Back.bmp \
    resources/skyboxs/2/bottom.bmp \
    resources/skyboxs/2/Front.bmp \
    resources/skyboxs/2/Left.bmp \
    resources/skyboxs/2/Right.bmp \
    resources/skyboxs/2/top.bmp \
    resources/skyboxs/3/Back.bmp \
    resources/skyboxs/3/bottom.bmp \
    resources/skyboxs/3/Front.bmp \
    resources/skyboxs/3/left.bmp \
    resources/skyboxs/3/right.bmp \
    resources/skyboxs/3/top.bmp \
    resources/textures/tauren.bmp \
    resources/pictures/audio.jpg \
    resources/pictures/material preview.jpg \
    resources/pictures/overview map.jpg \
    resources/skyboxs/4/back.jpg \
    resources/skyboxs/4/front.jpg \
    resources/skyboxs/4/left.jpg \
    resources/skyboxs/4/right.jpg \
    resources/skyboxs/4/top.jpg \
    resources/textures/default.jpg \
    resources/textures/grass.jpg \
    resources/textures/magma.jpg \
    resources/textures/rock.jpg \
    resources/textures/test1.jpg \
    resources/textures/test2.jpg \
    resources/textures/test3.jpg \
    resources/textures/test4.jpg \
    resources/icons/add.png \
    resources/icons/animation white.png \
    resources/icons/animation.png \
    resources/icons/arrow_right.png \
    resources/icons/arrows_stretch_horizontal.png \
    resources/icons/audio white.png \
    resources/icons/audio.png \
    resources/icons/audio2.png \
    resources/icons/branch-closed.png \
    resources/icons/branch-open.png \
    resources/icons/build.png \
    resources/icons/camera.png \
    resources/icons/camera2.png \
    resources/icons/close.png \
    resources/icons/cone.png \
    resources/icons/cone2.png \
    resources/icons/controller.png \
    resources/icons/controller2.png \
    resources/icons/cube.png \
    resources/icons/cube2.png \
    resources/icons/custom.png \
    resources/icons/cylinder.png \
    resources/icons/cylinder2.png \
    resources/icons/debug.png \
    resources/icons/directional light white.png \
    resources/icons/directional light.png \
    resources/icons/directional light2.png \
    resources/icons/earth.png \
    resources/icons/empty.png \
    resources/icons/empty2.png \
    resources/icons/eyedropper.png \
    resources/icons/face.png \
    resources/icons/float.png \
    resources/icons/fog.png \
    resources/icons/folder.png \
    resources/icons/folder2.png \
    resources/icons/folder3.png \
    resources/icons/folder4.png \
    resources/icons/folder5.png \
    resources/icons/geometry.png \
    resources/icons/geometry2.png \
    resources/icons/ground.png \
    resources/icons/hand.png \
    resources/icons/horizontal_arrow.png \
    resources/icons/invisible.png \
    resources/icons/invisible2.png \
    resources/icons/invisible3.png \
    resources/icons/invisible4.png \
    resources/icons/light.png \
    resources/icons/light2.png \
    resources/icons/list.png \
    resources/icons/lua.png \
    resources/icons/material white.png \
    resources/icons/material.png \
    resources/icons/material2.png \
    resources/icons/mesh white.png \
    resources/icons/mesh.png \
    resources/icons/model.png \
    resources/icons/moive.png \
    resources/icons/more.png \
    resources/icons/more2.png \
    resources/icons/move.png \
    resources/icons/move_horizontal.png \
    resources/icons/move_horizontal2.png \
    resources/icons/new.png \
    resources/icons/notice.png \
    resources/icons/open.png \
    resources/icons/particle white.png \
    resources/icons/particle.png \
    resources/icons/plane.png \
    resources/icons/plane2.png \
    resources/icons/play.png \
    resources/icons/point light white.png \
    resources/icons/point light.png \
    resources/icons/point light2.png \
    resources/icons/render.png \
    resources/icons/return.png \
    resources/icons/rotate.png \
    resources/icons/scale.png \
    resources/icons/scene white.png \
    resources/icons/scene.png \
    resources/icons/script white.png \
    resources/icons/script.png \
    resources/icons/script2.png \
    resources/icons/search.png \
    resources/icons/search2.png \
    resources/icons/search_close.png \
    resources/icons/search_close2.png \
    resources/icons/set.png \
    resources/icons/setting.png \
    resources/icons/skybox.png \
    resources/icons/skybox2.png \
    resources/icons/sphere.png \
    resources/icons/sphere2.png \
    resources/icons/spot light white.png \
    resources/icons/spot light.png \
    resources/icons/spot light2.png \
    resources/icons/switch_true.png \
    resources/icons/terrain.png \
    resources/icons/texture white.png \
    resources/icons/texture.png \
    resources/icons/transform.png \
    resources/icons/trigger.png \
    resources/icons/trigger2.png \
    resources/icons/visible.png \
    resources/icons/visible2.png \
    resources/icons/visible3.png \
    resources/icons/visible4.png \
    resources/pictures/height.png \
    resources/pictures/height2.png \
    resources/textures/1158133.png \
    resources/textures/close.png \
    resources/textures/onebit_31.png \
    resources/textures/testpng.png \
    resources/materials/base.mtl \
    resources/skyboxs/2/back.bmp.meta \
    resources/skyboxs/2/bottom.bmp.meta \
    resources/skyboxs/2/front.bmp.meta \
    resources/skyboxs/2/left.bmp.meta \
    resources/skyboxs/2/right.bmp.meta \
    resources/skyboxs/2/top.bmp.meta \
    resources/skyboxs/4/dsback.jpg.meta \
    resources/skyboxs/4/dsfront.jpg.meta \
    resources/skyboxs/4/dsleft.jpg.meta \
    resources/skyboxs/4/dsright.jpg.meta \
    resources/skyboxs/4/dstop.jpg.meta \
    resources/shaders/base.frag \
    resources/shaders/blin.frag \
    resources/shaders/blin_by_vertex.frag \
    resources/shaders/color.frag \
    resources/shaders/coordinate.frag \
    resources/shaders/diffuse.frag \
    resources/shaders/diffusewithcamera.frag \
    resources/shaders/doublediffuse.frag \
    resources/shaders/particle.frag \
    resources/shaders/standard.frag \
    resources/shaders/standard_outline.frag \
    resources/shaders/terrain.frag \
    resources/shaders/texture.frag \
    resources/shaders/base.vert \
    resources/shaders/blin_by_vertex.vert \
    resources/shaders/color.vert \
    resources/shaders/coordinate.vert \
    resources/shaders/diffuse.vert \
    resources/shaders/doublediffuse.vert \
    resources/shaders/particle.vert \
    resources/shaders/standard.vert \
    resources/shaders/terrain.vert \
    resources/shaders/texture.vert \
    resources/scripts/new script.lua
