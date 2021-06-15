#include "BBGLShader.h"
#include "BBVertexBufferObject.h"
#include <QOpenGLShaderProgram>
#include "BBUtils.h"
#include "BBGLBuffers.h"
#include "BBElementBufferObject.h"


BBGLShader::BBGLShader()
{
    m_pProgram = NULL;
    m_ProjectionMatrix.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
}

BBGLShader::~BBGLShader()
{
    BB_SAFE_DELETE(m_pProgram);
    BB_SAFE_DELETE(m_pElementBufferObject);

    QMap<QString, UniformTexture*>::Iterator texItr;
    for (texItr = m_mapUniformTextures.begin(); texItr != m_mapUniformTextures.end(); texItr++)
    {
        BB_SAFE_DELETE(*texItr);
    }

    QMap<QString, UniformVector4f*>::Iterator vectorItr;
    for (vectorItr = m_mapUniformVector4f.begin(); vectorItr != m_mapUniformVector4f.end(); vectorItr++)
    {
        BB_SAFE_DELETE(*vectorItr);
    }

    QMap<QString, UniformVector4fArray*>::Iterator vectorArrayItr;
    for (vectorArrayItr = m_mapUniformVector4fArray.begin(); vectorArrayItr != m_mapUniformVector4fArray.end(); vectorArrayItr++)
    {
        BB_SAFE_DELETE(*vectorArrayItr);
    }

    QMap<QString, UniformBool*>::Iterator boolItr;
    for (boolItr = m_mapUniformBool.begin(); boolItr != m_mapUniformBool.end(); boolItr++)
    {
        BB_SAFE_DELETE(*boolItr);
    }

    QMap<QString, UniformMatrix*>::Iterator matrixItr;
    for (matrixItr = m_mapUniformMatrix.begin(); matrixItr != m_mapUniformMatrix.end(); matrixItr++)
    {
        BB_SAFE_DELETE(*matrixItr);
    }
}

void BBGLShader::init(const QString &vertexShaderPath, const QString &fragmentShaderPath,
                      const unsigned short *pIndexes, int nIndexCount)
{
    // protected QOpenGLFunctions + initializeOpenGLFunctions in order to use OpenGL's extension mechanism
    // so that invoke glActiveTexture
    initializeOpenGLFunctions();

    int nFileSize;
    // path.toLatin1().data(); will cause Chinese garbled
    char *vertexShaderSource = BBUtils::loadFileContent(vertexShaderPath.toStdString().c_str(), nFileSize);
    char *fragmentShaderSource = BBUtils::loadFileContent(fragmentShaderPath.toStdString().c_str(), nFileSize);

    m_pProgram = new QOpenGLShaderProgram();
    m_pProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_pProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_pProgram->link();

    m_nPositionAttr = m_pProgram->attributeLocation(NAME_POSITION);
    m_nColorAttr = m_pProgram->attributeLocation(NAME_COLOR);
    m_nTexcoordAttr = m_pProgram->attributeLocation(NAME_TEXCOORD);
    m_nNormalAttr = m_pProgram->attributeLocation(NAME_NORMAL);
    m_nProjectionMatrixLocation = m_pProgram->uniformLocation(NAME_PROJECTIONMATRIX);
    m_nViewMatrixLocation = m_pProgram->uniformLocation(NAME_VIEWMATRIX);
    m_nModelMatrixLocation = m_pProgram->uniformLocation(NAME_MODELMATRIX);
    m_nITModelMatrixLocation = m_pProgram->uniformLocation(NAME_ITMODELMATRIX);

    m_pElementBufferObject = new BBElementBufferObject(pIndexes, nIndexCount);
}

void BBGLShader::render(const std::function<void()> &draw, const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix,
                        const QVector3D &cameraPos, BBVertexBufferObject *pVertexbuffer)
{
    render(draw, modelMatrix, viewMatrix, m_ProjectionMatrix, cameraPos, pVertexbuffer);
}

void BBGLShader::render(const std::function<void()> &draw, const QMatrix4x4 &modelMatrix, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix,
                        const QVector3D &cameraPos, BBVertexBufferObject *pVertexbuffer)
{
//    // camera movement
//    m_ViewMatrix = viewMatrix;
//    setVector4f(NAME_CAMERAPOSITION, cameraPos.x(), cameraPos.y(), cameraPos.z(), 1.0f);

//    m_ModelMatrix = modelMatrix;
//    m_ITModelMatrix = m_ModelMatrix.transposed().inverted();

//    m_pProgram->bind();
//    m_pElementBufferObject->bind();

//    m_pProgram->setUniformValue(m_nProjectionMatrixLocation, projectionMatrix);
//    m_pProgram->setUniformValue(m_nViewMatrixLocation, m_ViewMatrix);
//    m_pProgram->setUniformValue(m_nModelMatrixLocation, m_ModelMatrix);
//    m_pProgram->setUniformValue(m_nITModelMatrixLocation, m_ITModelMatrix);

//    int index = 0;
//    QMap<QString, UniformTexture*>::Iterator texItr;
//    for (texItr = m_mapUniformTextures.begin(); texItr != m_mapUniformTextures.end(); texItr++)
//    {
//        glActiveTexture(GL_TEXTURE0 + index);
//        glBindTexture(GL_TEXTURE_2D, (*texItr)->m_nTexture);
//        m_pProgram->setUniformValue((*texItr)->m_nLocation, index);
//        index++;
//    }

//    m_pProgram->enableAttributeArray(m_nPositionAttr);
//    m_pProgram->enableAttributeArray(m_nColorAttr);
//    m_pProgram->enableAttributeArray(m_nTexcoordAttr);
//    m_pProgram->enableAttributeArray(m_nNormalAttr);

////    glDrawArrays(GL_TRIANGLES, 0, 3);
//    // How many indexes to draw, data type, data starting position
////    glDrawElements(GL_QUADS, 6, GL_UNSIGNED_INT, 0);
//    draw();

//    m_pProgram->disableAttributeArray(m_nNormalAttr);
//    m_pProgram->disableAttributeArray(m_nTexcoordAttr);
//    m_pProgram->disableAttributeArray(m_nColorAttr);
//    m_pProgram->disableAttributeArray(m_nPositionAttr);

//    m_pElementBufferObject->release();
//    m_pProgram->release();
}

void BBGLShader::resize(float fWidth, float fHeight)
{
    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.perspective(50.0f, fWidth / fHeight, 0.1f, 1000.0f);
}

void BBGLShader::setVector4f(const QString &name, float x, float y, float z, float w)
{
    GLint location = m_pProgram->uniformLocation(name);
    if (location != -1)
    {
        UniformVector4f *v = new UniformVector4f();
        v->m_Vector4.setX(x);
        v->m_Vector4.setY(y);
        v->m_Vector4.setZ(z);
        v->m_Vector4.setW(w);
        v->m_nLocation = location;
        m_mapUniformVector4f.insert(name, v);
    }
}

void BBGLShader::setTexture(const QString &name, const QString &filePath, bool bInvertY)
{
    GLint location = m_pProgram->uniformLocation(name);
    if (location != -1)
    {
        UniformTexture *t = new UniformTexture();
        BBGLTexture2D *glTexture2D = new BBGLTexture2D(filePath, bInvertY);
        t->m_nTexture = glTexture2D->m_nTexture;
        t->m_nLocation = location;
        m_mapUniformTextures.insert(name, t);
    }
}

void BBGLShader::setTexture(const QString &name, int nSize)
{

}

void BBGLShader::setTexture(const QString &name, const GLuint &nTexture)
{

}

void BBGLShader::bindElementBufferObject(const unsigned short *pIndexes, int nIndexCount)
{
    m_pElementBufferObject->set(pIndexes, nIndexCount);
}




//void GLShader::setTexture(QString name, int size)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformTexture *t = new UniformTexture();
//        GLProcedureTexture *glProcedureTexture = new GLProcedureTexture(size);
//        t->m_texture = glProcedureTexture->m_texture;
//        t->m_location = location;
//        m_uniformTextures.insert(name, t);
//    }
//}

//void GLShader::setTexture(QString name, GLuint texture)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformTexture *t = new UniformTexture();
//        t->m_texture = texture;
//        t->m_location = location;
//        m_uniformTextures.insert(name, t);
//    }
//}



//void GLShader::setVector4fArray(QString name, QVector4D *vector4Array, int count)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformVector4fArray *v = new UniformVector4fArray();
//        v->vector4Array = vector4Array;
//        v->m_location = location;
//        v->count = count;
//        m_uniformVector4fArray.insert(name, v);
//    }
//}

//void GLShader::setBool(QString name, bool b)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformBool *v = new UniformBool();
//        v->m_location = location;
//        v->b = b;
//        m_uniformBool.insert(name, v);
//    }
//}

//void GLShader::setMatrix(QString name, QMatrix4x4 matrix)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformMatrix *v = new UniformMatrix();
//        v->m_matrix = matrix;
//        v->m_location = location;
//        m_uniformMatrix.insert(name, v);
//    }
//}
