#ifndef BBTRANSLATEFEEDBACKOBJECT_H
#define BBTRANSLATEFEEDBACKOBJECT_H


#include "BBBufferObject.h"

class BBTranslateFeedbackObject : public BBBufferObject
{
public:
    BBTranslateFeedbackObject(int nVertexCount, GLenum hint, GLenum eDrawPrimitiveType);

    void bind() override;
    void unbind() override;

    void debug();

private:
    GLuint createTFO();

    GLuint m_TFO;
    int m_nVertexCount;
    GLenum m_eDrawPrimitiveType;
};

#endif // BBTRANSLATEFEEDBACKOBJECT_H
