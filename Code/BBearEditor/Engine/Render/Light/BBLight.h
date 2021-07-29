#ifndef BBLIGHT_H
#define BBLIGHT_H


#include "Base/BBGameObject.h"

#define BB_FILENAME_DIRECTIONALLIGHT "directional light.png"
#define BB_FILENAME_POINTLIGHT "point light.png"
#define BB_FILENAME_SPOTLIGHT "spot light.png"

class BBScene;
class BBRenderPass;
class BBIcon;
class BBLightIndicator;

enum BBLightType
{
    Directional,
    Point,
    Spot
};

class BBLight : public BBGameObject
{
public:
    BBLight(BBScene *pScene);
    BBLight(BBScene *pScene, const QVector3D &position, const QVector3D &rotation, const QVector3D &scale);
    ~BBLight();

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;
    void setRotation(int nAngle, const QVector3D &axis, bool bUpdateLocalTransform = true) override;
    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;

    void setVisibility(bool bVisible) override;

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;
    void insertInRenderQueue(BBRenderQueue *pQueue) override;
    void removeFromRenderQueue(BBRenderQueue *pQueue) override;

    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const BBFrustum &frustum) override;

    void setRenderPass(BBRenderPass *pRenderPass);

    void setAmbientColor(float r, float g, float b, float a = 1.0f);
    void setDiffuseColor(float r, float g, float b, float a = 1.0f);
    void setSpecularColor(float r, float g, float b, float a = 1.0f);
    void setSetting0(float x, float y, float z, float w);
    void setSetting1(float x, float y, float z, float w);
    void setSetting2(float x, float y, float z, float w);

    virtual bool cull(BBCamera *pCamera, const QRectF &displayBox) = 0;
    virtual bool cull(BBCamera *pCamera, int nFrustumIndexX, int nFrustumIndexY) = 0;

public:
    inline float* getDiffuseColor() { return m_Diffuse; }

protected:
    void setHomogeneousPosition(const QVector3D &value, float w = 1.0f);

    BBLightType m_eType;
    BBScene *m_pScene;
    BBIcon *m_pIcon;
    BBLightIndicator *m_pIndicator;

    float m_HomogeneousPosition[4];
    float m_Ambient[4];
    float m_Diffuse[4];
    float m_Specular[4];
    unsigned int m_nCullingMask;

    float m_Setting0[4];
    float m_Setting1[4];
    float m_Setting2[4];
};

#endif // BBLIGHT_H


//class SpotLightIndicator : public Indicator
//{
//    void setSpotAngle(float angle);
//};

//    QVector4D lightDirection;
//    float spotAngle;
//    int spotLevel;

//};
