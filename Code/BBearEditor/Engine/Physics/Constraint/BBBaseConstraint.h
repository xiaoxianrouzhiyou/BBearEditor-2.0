#ifndef BBBASECONSTRAINT_H
#define BBBASECONSTRAINT_H


#include <QVector3D>

class BBBaseBody;

class BBBaseConstraint
{
public:
    BBBaseConstraint(BBBaseBody *pBody);
    virtual ~BBBaseConstraint();

    virtual void doConstraint(float fDeltaTime) = 0;

protected:
    BBBaseBody *m_pBody;
};

#endif // BBBASECONSTRAINT_H
