#ifndef BBAREALIGHT_H
#define BBAREALIGHT_H


#include <QVector3D>
#include "Geometry/BBTracer.h"

class BBAreaLight
{
public:
    BBAreaLight(int nMin0, int nMax0, int nMin1, int nMax1, int nFixedValue);

    void generatePhoton(QVector3D &origin, QVector3D &direction, float &fPowerScale, BBHitInfo &hitInfo);

private:
    // Define location and size
    // For example, an area light source in an xoz plane, m_nFixedValue means y,
    // m_nMin0 and m_nMax0 indicates the range of x, m_nMin1 and m_nMax1 indicates the range of z
    int m_nMin0;
    int m_nMax0;
    int m_nMin1;
    int m_nMax1;
    int m_nFixedValue;
};

#endif // BBAREALIGHT_H
