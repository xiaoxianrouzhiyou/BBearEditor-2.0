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

public:
    static bool contain(const BBPlane &left, const BBPlane &right,
                        const BBPlane &top, const BBPlane &bottom,
                        const BBPlane &front, const BBPlane &back,
                        const QVector3D &point);
    bool containWithInvertedRightAndTop(const BBPlane &left, const BBPlane &right,
                                        const BBPlane &top, const BBPlane &bottom,
                                        const QVector3D &pointOnFront, const QVector3D &pointOnBack,
                                        const QVector3D &point);

protected:
    int m_nTopLeftX;
    int m_nTopLeftY;
    int m_nWidth;
    int m_nHeight;

    BBPlane *m_pLeft;
    BBPlane *m_pRight;
    BBPlane *m_pTop;
    BBPlane *m_pBottom;
    BBPlane *m_pFront;
    BBPlane *m_pBack;
};


class BBFrustumCluster : public BBFrustum
{
public:
    BBFrustumCluster(BBCamera *pCamera,
                     int x, int y,
                     int nWidth, int nHeight,
                     int nCountX, int nCountY, int nCountZ);
    ~BBFrustumCluster();

    bool contain(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ, const QVector3D &point);
    bool containedInSphere(int nFrustumIndexX, int nFrustumIndexY, int nFrustumIndexZ,
                           const QVector3D &sphereCenter, float fSphereRadius);

private:
    void calculateXCrossSections(BBCamera *pCamera, int nCount);
    void calculateYCrossSections(BBCamera *pCamera, int nCount);
    void calculateZCrossSectionPoints(BBCamera *pCamera, int nCount);

    BBPlane *m_pXCrossSections;
    BBPlane *m_pYCrossSections;
    QVector3D *m_pZCrossSectionPoints;
};

#endif // BBFRUSTUM_H
