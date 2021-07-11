#ifndef BBCOORDINATESYSTEM2D_H
#define BBCOORDINATESYSTEM2D_H


#include "Base/BBGameObject.h"

class BBPositionCoordinateComponent2D;

class BBCoordinateSystem2D : public BBGameObject
{
protected:
    BBCoordinateSystem2D();

    virtual void transform(const BBRay &ray) = 0;

    BBAxisFlags m_SelectedAxis;
    QVector3D m_LastMousePos;
    BBGameObject *m_pSelectedObject;
    bool m_bTransforming;

public:
    void setSelectedObject(BBGameObject *pObject);
    virtual void setSelectedAxis(const BBAxisFlags &axis) = 0;
    virtual bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) = 0;
    void stopTransform();
};


class BBPositionCoordinateSystem2D : public BBCoordinateSystem2D
{
public:
    BBPositionCoordinateSystem2D();
    ~BBPositionCoordinateSystem2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setSelectedAxis(const BBAxisFlags &axis) override;
    bool mouseMoveEvent(const BBRay &ray, bool bMousePressed) override;

private:
    void transform(const BBRay &ray) override;

    BBPositionCoordinateComponent2D *m_pCoordinateComponent;
};

#endif // BBCOORDINATESYSTEM2D_H
