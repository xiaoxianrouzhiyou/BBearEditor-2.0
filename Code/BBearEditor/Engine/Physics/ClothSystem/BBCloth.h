#ifndef BBCLOTH_H
#define BBCLOTH_H


#include "Base/BBGameObject.h"

class BBClothMesh;
class BBClothBody;

class BBCloth : public BBGameObject
{
public:
    BBCloth(int nWidth = 10, int nHeight = 6);
    ~BBCloth();

    void init() override;

private:
    BBClothMesh *m_pClothMesh;
    BBClothBody *m_pClothBody;
};

#endif // BBCLOTH_H
