#include "BBGLShader.h"
#include "BBGLBuffers.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "BBUtils.h"

BBGLShader::BBGLShader()
{
    m_pProgram = NULL;
    m_ProjectionMatrix.perspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
}

BBGLShader::~BBGLShader()
{
    BB_SAFE_DELETE(m_pProgram);
    BB_SAFE_DELETE(m_pElementBufferObject);
}

void BBGLShader::init(const char *vertexShaderPath, const char *fragmentShaderPath,
                      const unsigned short *pIndexes, const int nIndexCount)
{
    int nFileSize;
    char *vertexShaderSource = BBUtils::loadFileContent(vertexShaderPath, nFileSize);
    char *fragmentShaderSource = BBUtils::loadFileContent(fragmentShaderPath, nFileSize);

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

    bindElementBufferObject(pIndexes, nIndexCount);
}

void BBGLShader::render(const std::function<void()> draw, const QMatrix4x4 modelMatrix, const QMatrix4x4 viewMatrix,
                        const QVector3D cameraPos, BBGLVertexBuffer *pVertexbuffer)
{
    render(draw, modelMatrix, viewMatrix, m_ProjectionMatrix, cameraPos, pVertexbuffer);
}

void BBGLShader::render(const std::function<void()> draw, const QMatrix4x4 modelMatrix, const QMatrix4x4 viewMatrix, const QMatrix4x4 projectionMatrix,
                        const QVector3D cameraPos, BBGLVertexBuffer *pVertexbuffer)
{
    // camera movement
    m_ViewMatrix = viewMatrix;
    setVector4f(NAME_CAMERAPOSITION, cameraPos.x(), cameraPos.y(), cameraPos.z(), 1.0f);

    m_ModelMatrix = modelMatrix;
    m_ITModelMatrix = m_ModelMatrix.transposed().inverted();

    m_pProgram->bind();
    m_pElementBufferObject->bind();

    m_pProgram->setUniformValue(m_nProjectionMatrixLocation, projectionMatrix);
    m_pProgram->setUniformValue(m_nViewMatrixLocation, m_ViewMatrix);
    m_pProgram->setUniformValue(m_nModelMatrixLocation, m_ModelMatrix);
    m_pProgram->setUniformValue(m_nITModelMatrixLocation, m_ITModelMatrix);

//    int index = 0;
//    QMap<QString, UniformTexture*>::Iterator texItr;
//    for (texItr = m_uniformTextures.begin(); texItr != m_uniformTextures.end(); texItr++)
//    {
//        glActiveTexture(GL_TEXTURE0 + index);
//        glBindTexture(GL_TEXTURE_2D, (*texItr)->m_texture);
//        m_program->setUniformValue((*texItr)->m_location, index);
//        index++;
//    }

//    //各种四维向量
//    QMap<QString, UniformVector4f*>::Iterator vectorItr;
//    for (vectorItr = m_uniformVector4f.begin(); vectorItr != m_uniformVector4f.end(); vectorItr++)
//    {
//        m_program->setUniformValue((*vectorItr)->m_location, (*vectorItr)->vector4);
//    }

//    //各种四维向量数组
//    QMap<QString, UniformVector4fArray*>::Iterator vectorArrayItr;
//    for (vectorArrayItr = m_uniformVector4fArray.begin(); vectorArrayItr != m_uniformVector4fArray.end(); vectorArrayItr++)
//    {
//        m_program->setUniformValueArray((*vectorArrayItr)->m_location, (*vectorArrayItr)->vector4Array,
//                                        (*vectorArrayItr)->count);
//    }

//    //各种布尔值
//    QMap<QString, UniformBool*>::Iterator boolItr;
//    for (boolItr = m_uniformBool.begin(); boolItr != m_uniformBool.end(); boolItr++)
//    {
//        m_program->setUniformValue((*boolItr)->m_location, (*boolItr)->b);
//    }

//    //各种矩阵
//    QMap<QString, UniformMatrix*>::Iterator matrixItr;
//    for (matrixItr = m_uniformMatrix.begin(); matrixItr != m_uniformMatrix.end(); matrixItr++)
//    {
//        m_program->setUniformValue((*matrixItr)->m_location, (*matrixItr)->m_matrix);
//    }


    m_pProgram->setAttributeArray(m_nPositionAttr, pVertexbuffer->getPosition(), 4, sizeof(float) * 4);
    m_pProgram->setAttributeArray(m_nColorAttr, pVertexbuffer->getColor(), 4, sizeof(float) * 4);
    m_pProgram->setAttributeArray(m_nTexcoordAttr, pVertexbuffer->getTexcoord(), 2, sizeof(float) * 2);
    m_pProgram->setAttributeArray(m_nNormalAttr, pVertexbuffer->getNormal(), 4, sizeof(float) * 4);

//    glVertexAttribPointer(m_nPositionAttr, 4, GL_FLOAT, GL_FALSE, 0, pVertexbuffer->getPosition());
//    glVertexAttribPointer(m_nColorAttr, 4, GL_FLOAT, GL_FALSE, 0, pVertexbuffer->getColor());
//    glVertexAttribPointer(m_nTexcoordAttr, 2, GL_FLOAT, GL_FALSE, 0, pVertexbuffer->getTexcoord());
//    glVertexAttribPointer(m_nNormalAttr, 4, GL_FLOAT, GL_FALSE, 0, pVertexbuffer->getNormal());

    m_pProgram->enableAttributeArray(m_nPositionAttr);
    m_pProgram->enableAttributeArray(m_nColorAttr);
    m_pProgram->enableAttributeArray(m_nTexcoordAttr);
    m_pProgram->enableAttributeArray(m_nNormalAttr);

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    // How many indexes to draw, data type, data starting position
//    glDrawElements(GL_QUADS, 6, GL_UNSIGNED_INT, 0);
    draw();

    m_pProgram->disableAttributeArray(m_nNormalAttr);
    m_pProgram->disableAttributeArray(m_nTexcoordAttr);
    m_pProgram->disableAttributeArray(m_nColorAttr);
    m_pProgram->disableAttributeArray(m_nPositionAttr);

    m_pElementBufferObject->release();
    m_pProgram->release();
}

void BBGLShader::resize(const float fWidth, const float fHeight)
{
    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.perspective(50.0f, fWidth / fHeight, 0.1f, 1000.0f);
}

void BBGLShader::bindElementBufferObject(const unsigned short *pIndexes, const int nIndexCount)
{
    m_pElementBufferObject = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pElementBufferObject->create();
    m_pElementBufferObject->bind();
    m_pElementBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_pElementBufferObject->allocate(pIndexes, sizeof(unsigned short) * nIndexCount);
    m_pElementBufferObject->release();
}

void BBGLShader::setVector4f(const QString name, const float x, const float y, const float z, const float w)
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

//void GLShader::setTexture(QString name, QString filePath, bool invertY)
//{
//    GLint location = m_program->uniformLocation(name);
//    if (location != -1)
//    {
//        UniformTexture *t = new UniformTexture();
//        GLTexture2D *glTexture2D = new GLTexture2D(filePath, invertY);
//        t->m_texture = glTexture2D->m_texture;
//        t->m_location = location;
//        m_uniformTextures.insert(name, t);
//    }
//}

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
