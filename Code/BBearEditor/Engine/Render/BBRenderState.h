#ifndef BBRENDERSTATE_H
#define BBRENDERSTATE_H


#include "BBBaseRenderComponent.h"

struct BBRenderState
{
    BBRenderState();
    void operator =(const BBRenderState &rs);

    GLenum m_PrimitiveType;
    bool m_bBlend;
    bool m_bZTest;
    bool m_bAlphaTest;
    bool m_bWriteR;
    bool m_bWriteG;
    bool m_bWriteB;
    bool m_bWriteA;
    bool m_bWriteZ;
    bool m_bWriteStencil;
    bool m_bCull;
    bool m_bEnablePointSprite;
    int m_CullFace;
    unsigned int m_AlphaTestFunc;
    float m_fAlphaTestValue;
    float m_fOffsetFactor;
    float m_fOffsetUnit;
    unsigned int m_SRCBlendFunc;
    unsigned int m_DSTBlendFunc;
    unsigned int m_ZTestFunc;
    int m_ClearStencilValue;
    unsigned int m_DrawFace;
    unsigned int m_PolygonMode;
    float m_fLineWidth;
};

class BBGlobalRenderState
{
public:
    static void init();
    static void updateBlendState(bool bEnable);
    static void updateBlendFunc(unsigned int src, unsigned int dst);
    static void updateZTestState(bool bEnable);
    static void updateAlphaTestState(bool bEnable);
    static void updateColorMask(bool r, bool g, bool b, bool a);
    static void updateZMask(bool bEnable);
    static void updateZFunc(unsigned int func);
    static void updatePolygonMode(unsigned int face, unsigned int mode);
    static void updateAlphaFunc(unsigned int func, float value);
    static void updateLineWidth(float fWidth);

private:
    static BBRenderState m_RenderState;
};

#endif // BBRENDERSTATE_H
