#ifndef BBPHOTONMAP_H
#define BBPHOTONMAP_H


#include <QVector3D>

struct BBPhoton
{
    QVector3D m_Position;
    // Incident
    QVector3D m_Direction;
    // Color
    QVector3D m_Power;
};

class BBPhotonMap
{
public:
    BBPhotonMap(int nMaxPhotonNum = 10000);
    ~BBPhotonMap();

    void store(const BBPhoton &photon);

    QVector3D* getPhotonPositions();
    int getPhotonNum() { return m_nPhotonNum; }
    QVector3D getBoxMin() { return m_BoxMin; }
    QVector3D getBoxMax() { return m_BoxMax; }

private:
    int m_nPhotonNum;
    int m_nMaxPhotonNum;
    BBPhoton *m_pPhoton;
    QVector3D m_BoxMin;
    QVector3D m_BoxMax;
};

#endif // BBPHOTONMAP_H
