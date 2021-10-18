#ifndef BBCLOTH_H
#define BBCLOTH_H


#include "Base/BBGameObject.h"

class BBClothMesh;
class BBClothBody;
class BBPBDSolver;

class BBCloth : public BBGameObject
{
public:
    BBCloth(const QVector3D &position, int nWidth = 10, int nHeight = 6);
    ~BBCloth();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setPosition(const QVector3D &position, bool bUpdateLocalTransform = true) override;

private:
    int m_nWidth;
    int m_nHeight;

    BBClothMesh *m_pClothMesh;
    BBClothBody *m_pClothBody;

    BBPBDSolver *m_pPBDSolver;
};

#endif // BBCLOTH_H
