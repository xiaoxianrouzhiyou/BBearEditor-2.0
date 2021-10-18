#ifndef BBFORCE_H
#define BBFORCE_H


class BBBaseBody;

class BBForce
{
public:
    BBForce();
    virtual ~BBForce();

    virtual void applyForce(BBBaseBody *pBody, float fDeltaTime) = 0;

protected:

};

#endif // BBFORCE_H
