#ifndef BBLIGHTINDICATOR_H
#define BBLIGHTINDICATOR_H


#include "Base/BBRenderableObject.h"

class BBLightIndicator : public BBRenderableObject
{
public:
    BBLightIndicator();
    BBLightIndicator(const QVector3D &position, const QVector3D &rotation);

    void init() override;
};


class BBDirectionalLightIndicator : public BBLightIndicator
{
public:
    BBDirectionalLightIndicator();
    BBDirectionalLightIndicator(const QVector3D &position, const QVector3D &rotation);

    void init() override;
    void render(BBCamera *pCamera) override;
};


class BBPointLightIndicator : public BBLightIndicator
{
public:
    BBPointLightIndicator();
    BBPointLightIndicator(const QVector3D &position);

    void init() override;
    void render(BBCamera *pCamera) override;
};


class BBSpotLightIndicator : public BBLightIndicator
{
public:
    BBSpotLightIndicator();
    BBSpotLightIndicator(const QVector3D &position, const QVector3D &rotation);

    void init() override;
    void render(BBCamera *pCamera) override;
};

#endif // BBLIGHTINDICATOR_H
