#pragma once

#ifndef BBBASERENDERCOMPONENT_H
#define BBBASERENDERCOMPONENT_H


#include <GL/glu.h>
#include <QOpenGLExtraFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QRgb>
#include <QImage>
#include <QMap>
#include "Utils/BBUtils.h"


#define NAME_POSITION "positionAttr"
#define NAME_COLOR "colorAttr"
#define NAME_TEXCOORD "texcoordAttr"
#define NAME_NORMAL "normalAttr"
#define NAME_PROJECTIONMATRIX "projectionMatrix"
#define NAME_VIEWMATRIX "viewMatrix"
#define NAME_MODELMATRIX "modelMatrix"
#define NAME_ITMODELMATRIX "IT_modelMatrix"
#define NAME_CAMERAPOSITION "cameraPositionUniform"
#define NAME_TEXTURE "textureUniform"


class BBBaseRenderComponent : protected QOpenGLExtraFunctions
{
public:
    BBBaseRenderComponent();
};

#endif // BBBASERENDERCOMPONENT_H
