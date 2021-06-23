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

    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                 const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                 const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                 const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3) override;

    void setRenderPass(BBRenderPass *pRenderPass);

    void setAmbientColor(float r, float g, float b, float a);
    void setDiffuseColor(float r, float g, float b, float a);
    void setSpecularColor(float r, float g, float b, float a);
    void setSetting0(float x, float y, float z, float w);
    void setSetting1(float x, float y, float z, float w);

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

    /**
     * @brief       point radius ...
     */
    float m_Setting0[4];
    float m_Setting1[4];
};

#endif // BBLIGHT_H



//class Circle : public RenderableObject
//{
//public:
//    Circle();
//    Circle(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    void init() override;

//private:
//    void draw() override;
//    unsigned short *mIndexes;
//    int mIndexCount;
//};

//class PointLightIndicator : public Indicator
//{
//public:
//    PointLightIndicator();
//    PointLightIndicator(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    void init() override;
//    void render(Camera camera) override;
//    void resize(float width, float height) override;
//    void setVisible(bool isVisible) override;

//private:
//    void draw() override;
//    Circle *mCircle;
//};

//class SpotLightIndicator : public Indicator
//{
//public:
//    SpotLightIndicator();
//    SpotLightIndicator(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    void init() override;
//    void setSpotAngle(float angle);

//private:
//    void draw() override;
//};



//class DirectionLight : public Light
//{
//public:
//    DirectionLight(Scene *scene);
//    DirectionLight(Scene *scene, float px, float py, float pz,
//                   float rx, float ry, float rz, float sx, float sy, float sz);
//    QVector4D getHomogeneousPosition();
//    void setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform = true) override;
//    void setRotation(QVector3D rotation, bool isUpdateLocalTransform = true) override;

//private:
//    void setHomogeneousPosition();
//    void setColor(int r, int g, int b) override;
//    QVector4D homogeneousPosition;
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
