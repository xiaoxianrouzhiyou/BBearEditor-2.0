#ifndef BBDIRECTIONALFORCE_H
#define BBDIRECTIONALFORCE_H


#include "BBForce.h"
#include <QVector3D>

class BBDirectionalForce : public BBForce
{
public:
    BBDirectionalForce(float x, float y, float z);
    BBDirectionalForce(const QVector3D &direction);

    void applyForce(BBBaseBody *pBody, float fDeltaTime) override;

private:
    QVector3D m_Direction;
};

#endif // BBDIRECTIONALFORCE_H
