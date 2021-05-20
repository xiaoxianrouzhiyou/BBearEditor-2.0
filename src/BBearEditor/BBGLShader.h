#ifndef BBGLSHADER_H
#define BBGLSHADER_H

#include <QtOpenGL>
#include <QVector4D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

#define NAME_POSITION "positionAttr"
#define NAME_COLOR "colorAttr"
#define NAME_TEXCOORD "texcoordAttr"
#define NAME_NORMAL "normalAttr"
#define NAME_PROJECTIONMATRIX "projectionMatrix"
#define NAME_VIEWMATRIX "viewMatrix"
#define NAME_MODELMATRIX "modelMatrix"
#define NAME_ITMODELMATRIX "IT_modelMatrix"
#define NAME_CAMERAPOSITION "cameraPositionUniform"
#define NAME_TEXTURE "textureUniform"

//enum Type {
//    COLOR = 0x01,
//    TEXCOORD = 0x02,
//    NORMAL = 0x04
//};
////位运算结果的类型
//Q_DECLARE_FLAGS(ShaderFlags, Type)
////重载|运算符
//Q_DECLARE_OPERATORS_FOR_FLAGS(ShaderFlags)

struct UniformTexture
{
    GLuint m_nLocation;
    GLuint m_nTexture;
    UniformTexture()
    {
        m_nLocation = -1;
        m_nTexture = 0;
    }
};

struct UniformVector4f
{
    GLuint m_nLocation;
    QVector4D m_Vector4;
    UniformVector4f()
    {
        m_nLocation = -1;
    }
};

struct UniformVector4fArray
{
    GLuint m_nLocation;
    QVector4D *m_pVector4Array;
    int m_nCount;
    UniformVector4fArray()
    {
        m_nLocation = -1;
    }
};

struct UniformBool
{
    GLuint m_nLocation;
    bool m_b;
    UniformBool()
    {
        m_nLocation = -1;
    }
};

struct UniformMatrix
{
    GLuint m_nLocation;
    QMatrix4x4 m_Matrix;
    UniformMatrix()
    {
        m_nLocation = -1;
    }
};

class BBGLVertexBuffer;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
class BBGLShader : protected QOpenGLFunctions
{
public:
    BBGLShader();
    virtual ~BBGLShader();

    void init(const QString &vertexShaderPath, const QString &fragmentShaderPath,
              const unsigned short *pIndexes = 0, int nIndexCount = 0);
    void render(const std::function<void()> &draw, const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix,
                const QVector3D &cameraPos, BBGLVertexBuffer *pVertexbuffer);
    void render(const std::function<void()> &draw, const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix,
                const QVector3D &cameraPos, BBGLVertexBuffer *pVertexbuffer);
    void resize(float fWidth, float fHeight);

    void setVector4f(const QString &name, float x, float y, float z, float w);
    void setTexture(const QString &name, const QString &filePath, bool bInvertY = true);
    void setTexture(const QString &name, int nSize);
    void setTexture(const QString &name, const GLuint &nTexture);

    void bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount);

private:
    QOpenGLShaderProgram *m_pProgram;

    QMatrix4x4 m_ProjectionMatrix;
    QMatrix4x4 m_ViewMatrix;
    QMatrix4x4 m_ModelMatrix;
    QMatrix4x4 m_ITModelMatrix;

    QOpenGLBuffer *m_pElementBufferObject;

    GLuint m_nPositionAttr;
    GLuint m_nColorAttr;
    GLuint m_nTexcoordAttr;
    GLuint m_nNormalAttr;

    GLuint m_nProjectionMatrixLocation;
    GLuint m_nViewMatrixLocation;
    GLuint m_nModelMatrixLocation;
    GLuint m_nITModelMatrixLocation;

    QMap<QString, UniformTexture*> m_mapUniformTextures;
    QMap<QString, UniformVector4f*> m_mapUniformVector4f;
    QMap<QString, UniformVector4fArray*> m_mapUniformVector4fArray;
    QMap<QString, UniformBool*> m_mapUniformBool;
    QMap<QString, UniformMatrix*> m_mapUniformMatrix;
};




//class GLShader
//{
//public:
//    void setVector4fArray(QString name, QVector4D *vector4Array, int count);
//    void setBool(QString name, bool b);
//    void setMatrix(QString name, QMatrix4x4 matrix);


//private:
//    //ShaderFlags m_flags;


//    GLuint m_lightViewLocation;
//    GLuint m_lightProjectionLocation;


//};


#endif // BBGLSHADER_H
