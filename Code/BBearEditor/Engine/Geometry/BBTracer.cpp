#include "BBTracer.h"
#include "3D/BBModel.h"
#include "Render/BBMaterial.h"
#include "BBPhotonMap.h"


int BBTracer::m_nMaxTraceDepth = 5;

void BBTracer::tracePhoton(const BBRay &ray, BBModel *pSceneModels[], int nModelCount, int depth, const QVector3D &power, BBPhotonMap *pPhotonMap)
{
    // After tracing many times, end
    BB_PROCESS_ERROR_RETURN((depth < m_nMaxTraceDepth));

    // need to record the info of hit point
    BBHitInfo nearHitInfo;
    // Find the nearest hit point and the corresponding model
    for (int i = 0; i < nModelCount; i++)
    {
        BBHitInfo hitInfo;
        if (pSceneModels[i]->hit(ray, 0.001f, FLT_MAX, hitInfo))
        {
            if (hitInfo.m_fDistance < nearHitInfo.m_fDistance)
            {
                nearHitInfo = hitInfo;
                nearHitInfo.m_pModel = pSceneModels[i];
            }
        }
    }

    if (nearHitInfo.m_pModel)
    {
        BBScatterInfo scatterInfo;
        if (nearHitInfo.m_pModel->getMesh()->getMaterial()->scatter(ray, nearHitInfo, scatterInfo))
        {
            // if it is specular, continue to trace photon, reflection or refraction
            if (scatterInfo.m_bSpecular)
            {
                tracePhoton(scatterInfo.m_SpecularRay, pSceneModels, nModelCount, depth + 1, power, pPhotonMap);
            }
            else
            {
                // It is assumed that the medium does not absorb photons
                BBPhoton photon;
                photon.m_Position = nearHitInfo.m_Position;
                photon.m_Direction = ray.getDirection();
                photon.m_Power = power;
                pPhotonMap->store(photon);
            }
        }
    }
}
