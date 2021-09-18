#ifndef BBTRACER_H
#define BBTRACER_H


#include "BBRay.h"
#include <cfloat>

class BBModel;
class BBPhotonMap;
class BBMaterial;

struct BBHitInfo
{
    BBHitInfo()
    {
        m_fDistance = FLT_MAX;
        m_pModel = nullptr;
    }

    QVector3D m_Position;
    QVector2D m_Texcoords;
    QVector3D m_Normal;
    float m_fDistance;
    BBModel *m_pModel;
};

struct BBScatterInfo
{
    BBScatterInfo()
    {
        m_bSpecular = false;
    }

    bool m_bSpecular;
    BBRay m_SpecularRay;
};

class BBTracer
{
public:
    static void tracePhoton(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth, const QVector3D &power, BBPhotonMap *pPhotonMap);

private:
    static int m_nMaxTraceDepth;
};

#endif // BBTRACER_H
