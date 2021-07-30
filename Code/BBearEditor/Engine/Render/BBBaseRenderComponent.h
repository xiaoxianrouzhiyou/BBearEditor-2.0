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


#define LOCATION_POSITION "BBPosition"
#define LOCATION_COLOR "BBColor"
#define LOCATION_TEXCOORD "BBTexcoord"
#define LOCATION_NORMAL "BBNormal"
#define LOCATION_PROJECTIONMATRIX "BBProjectionMatrix"
#define LOCATION_PROJECTIONMATRIX_I "BBProjectionMatrix_I"
#define LOCATION_VIEWMATRIX "BBViewMatrix"
#define LOCATION_VIEWMATRIX_I "BBViewMatrix_I"
#define LOCATION_MODELMATRIX "BBModelMatrix"
#define LOCATION_MODELMATRIX_IT "BBModelMatrix_IT"
#define LOCATION_CAMERA_POSITION "BBCameraPosition"
#define LOCATION_CANVAS "BBCanvas"
#define LOCATION_TEXTURE(x) "BBTexture"#x
#define LOCATION_TEXTURE_SETTING0 "BBTextureSettings"
#define LOCATION_LIGHT_POSITION "BBLightPosition"
#define LOCATION_LIGHT_COLOR "BBLightColor"
#define LOCATION_LIGHT_PROJECTIONMATRIX "BBLightProjectionMatrix"
#define LOCATION_LIGHT_VIEWMATRIX "BBLightViewMatrix"
#define LOCATION_LIGHT_SETTINGS(x) "BBLightSettings"#x
#define LOCATION_SCREEN_PARAMETERS "BBScreenParameters"
#define LOCATION_TIME "BBTime"
#define LOCATION_COLORFBO "BBColorFBO"
#define LOCATION_DEPTHFBO "BBDepthFBO"
#define LOCATION_SHADOWMap "BBShadowMap"


class BBBaseRenderComponent : protected QOpenGLExtraFunctions
{
public:
    BBBaseRenderComponent();
};

#endif // BBBASERENDERCOMPONENT_H
