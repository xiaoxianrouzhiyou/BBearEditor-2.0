#ifndef BBRENDERABLEOBJECT_H
#define BBRENDERABLEOBJECT_H

#include "BBGameObject.h"

class BBRenderableObject : public BBGameObject
{
public:
    BBRenderableObject();
    BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);
    ~BBRenderableObject();

    void setVisible(bool bVisible) { m_bVisible = bVisible; }

    virtual void init(QString path);
    void render(BBCamera *pCamera) override;
    void render(QMatrix4x4 modelMatrix, BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;
//    void setAmbientMaterial(float r, float g, float b, float a);
//    void setAmbientMaterial(QColor color);
//    void setDiffuseMaterial(float r, float g, float b, float a);
//    void setDiffuseMaterial(QColor color);
//    void setSpecularMaterial(float r, float g, float b, float a);
//    void setSpecularMaterial(QColor color);
//    void setTexture(QString imagePath, bool invertY = true);
//    void setTexture(int size);
//    void setTexture(GLuint texture);

protected:
    virtual void draw();
    bool m_bVisible;


//    GLShader mShader;
//    VertexBuffer *mVertexBuffer;

};

#endif // BBRENDERABLEOBJECT_H
