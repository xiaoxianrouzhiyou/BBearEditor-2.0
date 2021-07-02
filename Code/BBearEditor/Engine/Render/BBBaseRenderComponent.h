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


#define NAME_POSITION "position"
#define NAME_COLOR "color"
#define NAME_TEXCOORD "texcoord"
#define NAME_NORMAL "normal"
#define NAME_PROJECTIONMATRIX "projectionMatrix"
#define NAME_VIEWMATRIX "viewMatrix"
#define NAME_MODELMATRIX "modelMatrix"
#define NAME_ITMODELMATRIX "IT_modelMatrix"
#define NAME_CAMERA_POSITION "cameraPosition"
#define NAME_TEXTURE(x) "texture"#x
#define NAME_LIGHT_POSITION "lightPosition"
#define NAME_LIGHT_COLOR "lightColor"


class BBBaseRenderComponent : protected QOpenGLExtraFunctions
{
public:
    BBBaseRenderComponent();
};

#endif // BBBASERENDERCOMPONENT_H
