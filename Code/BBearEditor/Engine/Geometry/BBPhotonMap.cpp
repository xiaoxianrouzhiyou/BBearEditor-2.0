#include "BBPhotonMap.h"
#include "Utils/BBUtils.h"


BBPhotonMap::BBPhotonMap(int nMaxPhotonNum)
{
    m_nMaxPhotonNum = nMaxPhotonNum;
    m_nPhotonNum = 0;
    m_pPhoton = new BBPhoton[nMaxPhotonNum];
    m_BoxMin = QVector3D(1000000.0f, 1000000.0f, 1000000.0f);
    m_BoxMax = QVector3D(-1000000.0f, -1000000.0f, -1000000.0f);
}

BBPhotonMap::~BBPhotonMap()
{
    BB_SAFE_DELETE_ARRAY(m_pPhoton);
}

void BBPhotonMap::store(const BBPhoton &photon)
{
    BB_PROCESS_ERROR_RETURN((m_nPhotonNum < m_nMaxPhotonNum));
    m_pPhoton[m_nPhotonNum] = photon;
    m_nPhotonNum++;
    m_BoxMin = QVector3D(std::min(m_BoxMin.x(), photon.m_Position.x()), std::min(m_BoxMin.y(), photon.m_Position.y()), std::min(m_BoxMin.z(), photon.m_Position.z()));
    m_BoxMax = QVector3D(std::max(m_BoxMax.x(), photon.m_Position.x()), std::max(m_BoxMax.y(), photon.m_Position.y()), std::max(m_BoxMax.z(), photon.m_Position.z()));
}

QVector3D* BBPhotonMap::getPhotonPositions()
{
    QVector3D *pPositions = new QVector3D[m_nPhotonNum];
    for (int i = 0; i < m_nPhotonNum; i++)
    {
        pPositions[i] = m_pPhoton[i].m_Position;
    }
    return pPositions;
}
