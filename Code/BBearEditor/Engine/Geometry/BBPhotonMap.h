#ifndef BBPHOTONMAP_H
#define BBPHOTONMAP_H


#include <QVector3D>
#include "Utils/BBUtils.h"

struct BBPhoton
{
    QVector3D m_Position;
    // Incident
    QVector3D m_Direction;
    // Color
    QVector3D m_Power;
    int m_Axis;
};

class BBPhotonMap
{
public:
    BBPhotonMap(int nMaxPhotonNum = 10000);
    ~BBPhotonMap();

    BBPhotonMap operator=(const BBPhotonMap &photonMap);

    void store(const BBPhoton &photon);
    void balance();

    void debug();
    QVector3D* getPhotonPositions();
    int getPhotonNum() const { return m_nPhotonNum; }
    int getMaxPhotonNum() const { return m_nMaxPhotonNum; }
    BBPhoton* getPhoton() const { return m_pPhoton; }
    QVector3D getBoxMin() const { return m_BoxMin; }
    QVector3D getBoxMax() const { return m_BoxMax; }

private:
    void splitMedian(BBPhoton pPhoton[], int start, int end, int median, int axis);
    void balanceSegment(BBPhoton pPhoton[], int index, int start, int end);

    int m_nPhotonNum;
    int m_nMaxPhotonNum;
    BBPhoton *m_pPhoton;
    QVector3D m_BoxMin;
    QVector3D m_BoxMax;
};

#endif // BBPHOTONMAP_H
