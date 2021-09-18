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
    BBPhotonMap();
    ~BBPhotonMap();

    void store(const BBPhoton &photon);

private:
    int m_nPhotonNum;
    int m_nMaxPhotonNum;
    BBPhoton *m_pPhoton;
    QVector3D m_BoxMin;
    QVector3D m_BoxMax;
};

#endif // BBPHOTONMAP_H
