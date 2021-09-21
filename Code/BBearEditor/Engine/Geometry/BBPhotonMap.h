#ifndef BBPHOTONMAP_H
#define BBPHOTONMAP_H


#include <QVector3D>
#include "Utils/BBUtils.h"
#include "Geometry/BBRay.h"

struct BBPhoton
{
    QVector3D m_Position;
    // Incident
    QVector3D m_Direction;
    // Color
    QVector3D m_Power;
    int m_Axis;
    // for test
    bool m_bKNearestPhotons;

    BBPhoton()
    {
        m_bKNearestPhotons = false;
    }
};

struct BBNearestPhotons
{
    // Detection point position
    QVector3D m_DetectionPosition;
    // Maximum number of photons to find
    int m_nMaxPhotonCount;
    // Number of photons found
    int m_nCurrentCount;
    // Is the maximum heap full, m_nMaxPhotonCount == m_nCurrentCount
    bool m_bFulled;
    // Store the square of the distance in the photon maximum stack between photon and the detection point to avoid repeated calculation each time
    // [0] store the maximum
    float *m_pDistanceSquare;
    // Photons found
    BBPhoton **m_ppPhotons;

    BBNearestPhotons(const QVector3D &detectionPosition, int nMaxPhotonCount, float fDetectionDistance)
    {
        m_DetectionPosition = detectionPosition;
        m_nMaxPhotonCount = nMaxPhotonCount;
        m_nCurrentCount = 0;
        m_bFulled = false;
        m_pDistanceSquare = new float[m_nMaxPhotonCount + 1];
        m_ppPhotons = new BBPhoton*[m_nMaxPhotonCount + 1];
        m_pDistanceSquare[0] = fDetectionDistance * fDetectionDistance;
    }

    ~BBNearestPhotons()
    {
        BB_SAFE_DELETE_ARRAY(m_pDistanceSquare);
        BB_SAFE_DELETE_ARRAY(m_ppPhotons);
    }
};

class BBPhotonMap
{
public:
    BBPhotonMap(int nMaxPhotonNum = 10000);
    ~BBPhotonMap();

    BBPhotonMap operator=(const BBPhotonMap &photonMap);

    void store(const BBPhoton &photon);
    void balance();
    void getKNearestPhotons(BBNearestPhotons *pNearestPhotons, int nParentIndex);
    QVector3D getIrradiance(const QVector3D &detectionPosition, const QVector3D &detectionNormal, float fDetectionDistance, int nMaxPhotonCount);

    // Test func
    void debug();
    void debug(BBNearestPhotons *pNearestPhotons);
    void markKNearestPhotons(BBNearestPhotons *pNearestPhotons);
    bool isMarkedKNearestPhotons(int nIndex);

    QVector3D* getPhotonPositions();
    int getPhotonNum() const { return m_nPhotonNum; }
    int getMaxPhotonNum() const { return m_nMaxPhotonNum; }
    BBPhoton* getPhoton() const { return m_pPhoton; }
    QVector3D getBoxMin() const { return m_BoxMin; }
    QVector3D getBoxMax() const { return m_BoxMax; }

public:
    static void tracePhoton(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth, const QVector3D &power, BBPhotonMap *pPhotonMap, bool bOnlyStoreCausticsPhoton);
    static QVector3D traceRay(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth, BBPhotonMap *pPhotonMap);

private:
    static int m_nMaxTraceDepth;

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
