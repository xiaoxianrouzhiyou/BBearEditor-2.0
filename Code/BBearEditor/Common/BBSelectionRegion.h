#ifndef BBSELECTIONREGION_H
#define BBSELECTIONREGION_H


#include <QVector3D>


class BBSelectionRegion
{
public:
    BBSelectionRegion();

    void setRect(float x, float y, float w, float h);
    void render();
    void setVisibility(bool bVisible);

private:
    QVector3D m_Vertexes[4];
    bool m_bVisible;
};

#endif // BBSELECTIONREGION_H
