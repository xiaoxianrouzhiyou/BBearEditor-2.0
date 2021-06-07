#include "BBSelectionRegion.h"
#include <QtOpenGL>

BBSelectionRegion::BBSelectionRegion()
{
    m_bVisible = false;
}

void BBSelectionRegion::setRect(float x, float y, float w, float h)
{
    // x y means left-bottom
    m_Vertexes[0].setX(x);
    m_Vertexes[0].setY(y);
    m_Vertexes[1].setX(x + w);
    m_Vertexes[1].setY(y);
    m_Vertexes[2].setX(x + w);
    m_Vertexes[2].setY(y + h);
    m_Vertexes[3].setX(x);
    m_Vertexes[3].setY(y + h);
}

void BBSelectionRegion::render()
{
    if (m_bVisible)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glShadeModel(GL_SMOOTH);
        // 2D images do not receive lighting algorithms
        glDisable(GL_LIGHTING);
        // Rect
        glColor4ub(214, 223, 235, 50);
        glBegin(GL_QUADS);
        glVertex3f(m_Vertexes[0].x(), m_Vertexes[0].y(), m_Vertexes[0].z());
        glVertex3f(m_Vertexes[1].x(), m_Vertexes[1].y(), m_Vertexes[1].z());
        glVertex3f(m_Vertexes[2].x(), m_Vertexes[2].y(), m_Vertexes[2].z());
        glVertex3f(m_Vertexes[3].x(), m_Vertexes[3].y(), m_Vertexes[3].z());
        glEnd();
        glDisable(GL_BLEND);
    }
}

void BBSelectionRegion::setVisibility(bool bVisible)
{
    m_bVisible = bVisible;
}
