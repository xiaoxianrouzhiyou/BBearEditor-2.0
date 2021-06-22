#ifndef BBLIGHT_H
#define BBLIGHT_H


#include "Base/BBGameObject.h"

class BBScene;
class BBIcon;

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

    void init(const QString &path) override;
    void render(BBCamera *pCamera) override;

    bool hit(const BBRay &ray, float &fDistance) override;
    bool belongToSelectionRegion(const QVector3D &left1, const QVector3D &left2, const QVector3D &left3,
                                 const QVector3D &top1, const QVector3D &top2, const QVector3D &top3,
                                 const QVector3D &right1, const QVector3D &right2, const QVector3D &right3,
                                 const QVector3D &bottom1, const QVector3D &bottom2, const QVector3D &bottom3) override;

    void setAmbientColor(float r, float g, float b, float a);
    void setDiffuseColor(float r, float g, float b, float a);
    void setSpecularColor(float r, float g, float b, float a);
    void setSetting0(float x, float y, float z, float w);
    void setSetting1(float x, float y, float z, float w);

protected:
    static BBLight *m_pMainLight;

protected:
    BBLightType m_eType;
    BBScene *m_pScene;
    BBIcon *m_pIcon;

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



//class Indicator : public RenderableObject
//{
//public:
//    Indicator();
//    Indicator(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

//protected:
//    unsigned short *mIndexes;
//    int mIndexCount;
//};

//class DirectionLightIndicator : public Indicator
//{
//public:
//    DirectionLightIndicator();
//    DirectionLightIndicator(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
//    void render(Camera camera) override;
//    void init() override;

//private:
//    void draw() override;
//};


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


//class Light : public GameObject
//{
//public:
//    void init(QString fileName);
//    void render(Camera camera) override;
//    bool hit(Ray ray, float &distance) override;
//    bool belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                 QVector3D top1, QVector3D top2, QVector3D top3,
//                                 QVector3D right1, QVector3D right2, QVector3D right3,
//                                 QVector3D bottom1, QVector3D bottom2, QVector3D bottom3) override;
//    void setPosition(QVector3D position, bool isUpdateLocalTransform = true) override;
//    void setRotation(int angle, QVector3D axis, bool isUpdateLocalTransform = true) override;
//    void setRotation(QVector3D rotation, bool isUpdateLocalTransform = true) override;
//    void setVisible(bool isVisible) override;
//    QColor getColor();
//    QVector4D getColorVector4f();
//    virtual void setColor(int r, int g, int b);
//    Indicator *mIndicator;

//protected:
//    QColor mColor;
//    QVector4D mColorVector4f;
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