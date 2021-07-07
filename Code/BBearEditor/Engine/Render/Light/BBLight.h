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

    virtual bool cull(BBCamera *pCamera, const QRectF &displayBox) = 0;

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
};

#endif // BBLIGHT_H


//class SpotLightIndicator : public Indicator
//{
//    void setSpotAngle(float angle);
//};


//class PointLight : public Light
//{
//public:
//    PointLight(Scene *scene);
//    PointLight(Scene *scene, float px, float py, float pz,
//               float rx, float ry, float rz, float sx, float sy, float sz);
//    virtual void setConstantFactor(float value);
//    float getConstantFactor();
//    virtual void setLinearFactor(float value);
//    float getLinearFactor();
//    virtual void setQuadricFactor(float value);
//    float getQuadricFactor();
//    virtual QVector4D getOptionVector4f();
//    int getIntensity();
//    virtual void setIntensity(int intensity);
//    static float attenuationThreshold;

//protected:
//    void updateValidRange();
//    float constantFactor;
//    float linearFactor;
//    float quadricFactor;
//    int mIntensity;

//private:
//    void setPosition(QVector3D position, bool isUpdateLocalTransform = true) override;
//    void setColor(int r, int g, int b) override;
//};

////聚光灯是对点光源进行约束
//class SpotLight : public PointLight
//{
//public:
//    SpotLight(Scene *scene);
//    SpotLight(Scene *scene, float px, float py, float pz,
//              float rx, float ry, float rz, float sx, float sy, float sz);
//    void setConstantFactor(float value) override;
//    void setLinearFactor(float value) override;
//    void setQuadricFactor(float value) override;
//    QVector4D getOptionVector4f() override;
//    QVector4D getOption2Vector4f();
//    QVector4D getLightDirection();
//    float getSpotAngle();
//    void setSpotAngle(float angle);
//    int getSpotLevel();
//    void setSpotLevel(int level);
//    void setIntensity(int intensity) override;

//private:
//    void setPosition(QVector3D position, bool isUpdateLocalTransform = true) override;
//    void setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform = true) override;
//    void setRotation(QVector3D rotation, bool isUpdateLocalTransform = true) override;
//    void setLightDirection();
//    void setColor(int r, int g, int b) override;
//    QVector4D lightDirection;
//    float spotAngle;
//    int spotLevel;

//};
