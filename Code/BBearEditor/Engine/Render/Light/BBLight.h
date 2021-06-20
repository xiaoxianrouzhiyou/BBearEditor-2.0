#ifndef BBLIGHT_H
#define BBLIGHT_H


enum BBLightType
{
    Directional,
    Point,
    Spot
};

class BBLight
{
public:
    BBLight();
    ~BBLight();

    void setAmbientColor(float r, float g, float b, float a);
    void setDiffuseColor(float r, float g, float b, float a);
    void setSpecularColor(float r, float g, float b, float a);
    void setSetting0(float x, float y, float z, float w);
    void setSetting1(float x, float y, float z, float w);

private:
    static BBLight *m_pMainLight;

private:
    BBLightType m_eType;
    float m_Ambient[4];
    float m_Diffuse[4];
    float m_Specular[4];
    unsigned int m_nCullingMask;

    /**
     * @brief       point radius
     */
    float m_Setting0[4];
    float m_Setting1[4];
};

#endif // BBLIGHT_H
