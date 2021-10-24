#pragma once

#ifndef BBBASERENDERCOMPONENT_H
#define BBBASERENDERCOMPONENT_H


#include <GL/glu.h>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
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
#define LOCATION_TANGENT "BBTangent"
#define LOCATION_BITANGENT "BBBiTangent"
#define LOCATION_SMOOTHNORMAL "BBSmoothNormal"
#define LOCATION_PROJECTIONMATRIX "BBProjectionMatrix"
#define LOCATION_PROJECTIONMATRIX_I "BBProjectionMatrix_I"
#define LOCATION_VIEWMATRIX "BBViewMatrix"
#define LOCATION_VIEWMATRIX_I "BBViewMatrix_I"
#define LOCATION_MODELMATRIX "BBModelMatrix"
#define LOCATION_MODELMATRIX_IT "BBModelMatrix_IT"
#define LOCATION_VIEWMODELMATRIX_IT "BBViewModelMatrix_IT"
#define LOCATION_CAMERA_POSITION "BBCameraPosition"
#define LOCATION_CANVAS "BBCanvas"
#define LOCATION_TEXTURE(x) "BBTexture"#x
#define LOCATION_TEXTURE_SETTING0 "BBTextureSettings"
#define LOCATION_LIGHT_POSITION "BBLightPosition"
#define LOCATION_LIGHT_COLOR "BBLightColor"
#define LOCATION_LIGHT_PROJECTIONMATRIX "BBLightProjectionMatrix"
#define LOCATION_LIGHT_VIEWMATRIX "BBLightViewMatrix"
#define LOCATION_LIGHT_SETTINGS(x) "BBLightSettings"#x
#define LOCATION_CAMERA_PARAMETERS0 "BBCameraParameters"
#define LOCATION_CAMERA_PARAMETERS1 "BBCameraParameters1"
#define LOCATION_TIME "BBTime"
#define LOCATION_CAMERA_COLOR_TEXTURE "BBCameraColorTexture"
#define LOCATION_CAMERA_DEPTH_TEXTURE "BBCameraDepthTexture"
#define LOCATION_SHADOWMAP "BBShadowMap"
#define LOCATION_SKYBOX_MAP "BBSkyBox"
#define LOCATION_IRRADIANCE_MAP "BBIrradianceMap"
#define LOCATION_PREFILTER_MAP_MIPMAP "BBPrefilterMapMipmap"
#define LOCATION_SKYBOX_EQUIRECTANGULAR_MAP "BBSkyBoxEquirectangularMap"
#define LOCATION_BRDF_LUT_TEXTURE "BBBRDFLUTTexture"
#define LOCATION_SPHERICAL_HARMONIC_LIGHTING "BBSphericalHarmonicLightingCoefficients[0]"

#define LOCATION_COLOR_PREFIX "BBColor_"
#define LOCATION_COLOR_PREFIX_CHAR_COUNT 8
#define LOCATION_TILINGANDOFFSET_PREFIX "BBTO_"
#define LOCATION_TILINGANDOFFSET_PREFIX_CHAR_COUNT 5

#define FBO_COLOR_BUFFER_NAME(x) "BBFBOColor"#x
#define FBO_DEPTH_BUFFER_NAME "BBFBODepth"
#define INDEX_SHADOWMAP 2


class BBBaseRenderComponent : protected QOpenGLFunctions_4_5_Core
{
public:
    BBBaseRenderComponent() { initializeOpenGLFunctions(); }
};

#endif // BBBASERENDERCOMPONENT_H
