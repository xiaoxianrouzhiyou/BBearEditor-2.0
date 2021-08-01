#ifndef BBNORMALINDICATOR_H
#define BBNORMALINDICATOR_H


#include "Base/BBRenderableObject.h"

class BBNormalIndicator : public BBRenderableObject
{
public:
    BBNormalIndicator();

    void init(BBRenderableObject *pObject);

private:
    void clear();
};

#endif // BBNORMALINDICATOR_H
