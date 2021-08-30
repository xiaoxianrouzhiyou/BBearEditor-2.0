#include "BBRenderState.h"


#define SetBlendState(bEnable) \
do { \
    bEnable? glEnable(GL_BLEND) : glDisable(GL_BLEND); \
} while(0)

#define SetZTestState(bEnable) \
do { \
    bEnable? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST); \
} while(0)

#define SetAlphaTestState(bEnable) \
do { \
    bEnable? glEnable(GL_ALPHA_TEST) : glDisable(GL_ALPHA_TEST); \
} while(0)

#define SetCullState(bEnable) \
do { \
    bEnable? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE); \
} while(0)

#define SetPointSpriteState(bEnable) \
do { \
    bEnable? glEnable(GL_POINT_SPRITE) : glDisable(GL_POINT_SPRITE); \
} while(0)

#define SetProgramPointSizeState(bEnable) \
do { \
    bEnable? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE); \
} while(0)

#define SetCubeMapSeamlessState(bEnable) \
do { \
    bEnable? glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) : glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS); \
} while(0)


/**
 * @brief BBRenderState::BBRenderState
 */
BBRenderState::BBRenderState()
{
    m_PrimitiveType = GL_TRIANGLES;
    m_bBlend = false;
    m_bZTest = true;
    m_bAlphaTest = false;
    m_ZTestFunc = GL_LEQUAL;
    m_bWriteR = true;
    m_bWriteG = true;
    m_bWriteB = true;
    m_bWriteZ = true;
    m_bWriteStencil = false;
    m_bUseStencil = false;
    m_bCull = false;
    m_CullFace = GL_BACK;
    m_bEnablePointSprite = true;
    m_bEnableProgramPointSize = true;
    m_AlphaTestFunc = GL_GREATER;
    m_fAlphaTestValue = 0.1f;
    m_fOffsetFactor = 0.0f;
    m_fOffsetUnit = 0.0f;
    m_SRCBlendFunc = GL_SRC_ALPHA;
    m_DSTBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
    m_ClearStencilValue = -1;
    m_DrawFace = GL_FRONT;
    m_PolygonMode = GL_FILL;
    m_fLineWidth = 1.0f;
    m_bCubeMapSeamless = true;
}

void BBRenderState::operator =(const BBRenderState &rs)
{
    memcpy(this, &rs, sizeof(BBRenderState));
}


/**
 * @brief BBGlobalRenderState::BBGlobalRenderState
 */

BBRenderState BBGlobalRenderState::m_RenderState;

void BBGlobalRenderState::init()
{
    SetBlendState(m_RenderState.m_bBlend);
    glBlendFunc(m_RenderState.m_SRCBlendFunc, m_RenderState.m_DSTBlendFunc);
    SetZTestState(m_RenderState.m_bZTest);
    SetAlphaTestState(m_RenderState.m_bAlphaTest);
    glColorMask(m_RenderState.m_bWriteR,
                m_RenderState.m_bWriteG,
                m_RenderState.m_bWriteB,
                m_RenderState.m_bWriteA);
    glDepthMask(m_RenderState.m_bWriteZ);
    glDepthFunc(m_RenderState.m_ZTestFunc);
    glPolygonMode(m_RenderState.m_DrawFace, m_RenderState.m_PolygonMode);
    glAlphaFunc(m_RenderState.m_AlphaTestFunc, m_RenderState.m_fAlphaTestValue);
    glLineWidth(m_RenderState.m_fLineWidth);
    SetPointSpriteState(m_RenderState.m_bEnablePointSprite);
    SetProgramPointSizeState(m_RenderState.m_bEnableProgramPointSize);
    SetCullState(m_RenderState.m_bCull);
    glCullFace(m_RenderState.m_CullFace);
    // Correct filtering of cube map
    SetCubeMapSeamlessState(m_RenderState.m_bCubeMapSeamless);
}

void BBGlobalRenderState::updateBlendState(bool bEnable)
{
    if (m_RenderState.m_bBlend != bEnable)
    {
        m_RenderState.m_bBlend = bEnable;
        SetBlendState(m_RenderState.m_bBlend);
    }
}

void BBGlobalRenderState::updateBlendFunc(unsigned int src, unsigned int dst)
{
    if (m_RenderState.m_SRCBlendFunc != src || m_RenderState.m_DSTBlendFunc != dst)
    {
        m_RenderState.m_SRCBlendFunc = src;
        m_RenderState.m_DSTBlendFunc = dst;
        glBlendFunc(m_RenderState.m_SRCBlendFunc, m_RenderState.m_DSTBlendFunc);
    }
}

void BBGlobalRenderState::updateZTestState(bool bEnable)
{
    if (m_RenderState.m_bZTest != bEnable)
    {
        m_RenderState.m_bZTest = bEnable;
        SetZTestState(m_RenderState.m_bZTest);
    }
}

void BBGlobalRenderState::updateAlphaTestState(bool bEnable)
{
    if (m_RenderState.m_bAlphaTest != bEnable)
    {
        m_RenderState.m_bAlphaTest = bEnable;
        SetAlphaTestState(m_RenderState.m_bAlphaTest);
    }
}

void BBGlobalRenderState::updateColorMask(bool r, bool g, bool b, bool a)
{
    if (m_RenderState.m_bWriteR != r || m_RenderState.m_bWriteG != g
            || m_RenderState.m_bWriteB != b || m_RenderState.m_bWriteA != a)
    {
        m_RenderState.m_bWriteR = r;
        m_RenderState.m_bWriteG = g;
        m_RenderState.m_bWriteB = b;
        m_RenderState.m_bWriteA = a;
        glColorMask(m_RenderState.m_bWriteR,
                    m_RenderState.m_bWriteG,
                    m_RenderState.m_bWriteB,
                    m_RenderState.m_bWriteA);
    }
}

void BBGlobalRenderState::updateStencilMask(bool bEnable)
{
    if (m_RenderState.m_bWriteStencil != bEnable)
    {
        m_RenderState.m_bWriteStencil = bEnable;
        glStencilMask(bEnable);
    }
}

void BBGlobalRenderState::updateZMask(bool bEnable)
{
    if (m_RenderState.m_bWriteZ != bEnable)
    {
        m_RenderState.m_bWriteZ = bEnable;
        glDepthMask(m_RenderState.m_bWriteZ);
    }
}

void BBGlobalRenderState::updateZFunc(unsigned int func)
{
    if (m_RenderState.m_ZTestFunc != func)
    {
        m_RenderState.m_ZTestFunc = func;
        glDepthFunc(m_RenderState.m_ZTestFunc);
    }
}

void BBGlobalRenderState::updatePolygonMode(unsigned int face, unsigned int mode)
{
    if (m_RenderState.m_DrawFace != face || m_RenderState.m_PolygonMode != mode)
    {
        m_RenderState.m_DrawFace = face;
        m_RenderState.m_PolygonMode = mode;
        glPolygonMode(m_RenderState.m_DrawFace, m_RenderState.m_PolygonMode);
    }
}

void BBGlobalRenderState::updateAlphaFunc(unsigned int func, float value)
{
    if (m_RenderState.m_AlphaTestFunc != func || m_RenderState.m_fAlphaTestValue != value)
    {
        m_RenderState.m_AlphaTestFunc = func;
        m_RenderState.m_fAlphaTestValue = value;
        glAlphaFunc(m_RenderState.m_AlphaTestFunc, m_RenderState.m_fAlphaTestValue);
    }
}

void BBGlobalRenderState::updateLineWidth(float fWidth)
{
    if (m_RenderState.m_fLineWidth != fWidth)
    {
        m_RenderState.m_fLineWidth = fWidth;
        glLineWidth(m_RenderState.m_fLineWidth);
    }
}

void BBGlobalRenderState::updateCullState(bool bEnable)
{
    if (m_RenderState.m_bCull != bEnable)
    {
        m_RenderState.m_bCull = bEnable;
        SetCullState(m_RenderState.m_bCull);
    }
}

void BBGlobalRenderState::updateCullFace(int face)
{
    if (m_RenderState.m_CullFace != face)
    {
        m_RenderState.m_CullFace = face;
        glCullFace(m_RenderState.m_CullFace);
    }
}

void BBGlobalRenderState::updatePointSpriteState(bool bEnable)
{
    if (m_RenderState.m_bEnablePointSprite != bEnable)
    {
        m_RenderState.m_bEnablePointSprite = bEnable;
        SetPointSpriteState(m_RenderState.m_bEnablePointSprite);
    }
}

void BBGlobalRenderState::updateProgramPointSizeState(bool bEnable)
{
    if (m_RenderState.m_bEnableProgramPointSize != bEnable)
    {
        m_RenderState.m_bEnableProgramPointSize = bEnable;
        SetProgramPointSizeState(m_RenderState.m_bEnableProgramPointSize);
    }
}

void BBGlobalRenderState::clearStencil(int value)
{
    if (m_RenderState.m_ClearStencilValue != value)
    {
        m_RenderState.m_ClearStencilValue = value;
        glClearStencil(m_RenderState.m_ClearStencilValue);
    }
}
