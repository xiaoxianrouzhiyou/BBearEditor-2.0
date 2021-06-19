#pragma once

#ifndef BBBASERENDERCOMPONENT_H
#define BBBASERENDERCOMPONENT_H


#include <GL/glu.h>
#include <QOpenGLFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QRgb>
#include "BBUtils.h"
#include <QImage>
#include <QMap>
#include "BBUtils.h"


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


class BBBaseRenderComponent : protected QOpenGLFunctions
{
public:
    BBBaseRenderComponent();
};

#endif // BBBASERENDERCOMPONENT_H
