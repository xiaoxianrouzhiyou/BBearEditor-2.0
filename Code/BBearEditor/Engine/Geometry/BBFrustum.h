#ifndef BBFRUSTUM_H
#define BBFRUSTUM_H


#include <QVector3D>

class BBCamera;
class BBPlane;
class BBGameObject;

class BBFrustum
{
public:
    BBFrustum(BBCamera *pCamera, int x, int y, int nWidth, int nHeight);
    ~BBFrustum();

    bool contain(const QVector3D &point) const;

private:
    BBPlane *m_pLeft;
    BBPlane *m_pRight;
    BBPlane *m_pTop;
    BBPlane *m_pBottom;
    BBPlane *m_pFront;
    BBPlane *m_pBack;
};

#endif // BBFRUSTUM_H
