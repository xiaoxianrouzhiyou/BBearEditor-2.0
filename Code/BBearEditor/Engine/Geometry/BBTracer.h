#ifndef BBTRACER_H
#define BBTRACER_H


#include "BBRay.h"

class BBPhotonMap;
class BBMaterial;

class BBTracer
{
public:
    static void tracePhoton(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth, const QVector3D &power, BBPhotonMap *pPhotonMap);
    static QVector3D traceRay(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth);

private:
    static int m_nMaxTraceDepth;
};

#endif // BBTRACER_H
