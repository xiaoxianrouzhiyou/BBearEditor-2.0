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


#define LOCATION_POSITION "position"
#define LOCATION_COLOR "color"
#define LOCATION_TEXCOORD "texcoord"
#define LOCATION_NORMAL "normal"
#define LOCATION_PROJECTIONMATRIX "projectionMatrix"
#define LOCATION_PROJECTIONMATRIX_I "projectionMatrix_I"
#define LOCATION_VIEWMATRIX "viewMatrix"
#define LOCATION_VIEWMATRIX_I "viewMatrix_I"
#define LOCATION_MODELMATRIX "modelMatrix"
#define LOCATION_ITMODELMATRIX "IT_modelMatrix"
#define LOCATION_CAMERA_POSITION "cameraPosition"
#define LOCATION_CANVAS "canvas"
#define LOCATION_TEXTURE(x) "texture"#x
#define LOCATION_TEXTURE_SETTING0 "textureSettings"
#define LOCATION_LIGHT_POSITION "lightPosition"
#define LOCATION_LIGHT_COLOR "lightColor"
#define LOCATION_LIGHT_SETTINGS(x) "lightSettings"#x
#define LOCATION_SCREEN_PARAMETERS "screenParameters"
#define LOCATION_TIME "time"
#define LOCATION_COLORFBO "ColorFBO"
#define LOCATION_DEPTHFBO "DepthFBO"


class BBBaseRenderComponent : protected QOpenGLExtraFunctions
{
public:
    BBBaseRenderComponent();
};

#endif // BBBASERENDERCOMPONENT_H
