#ifndef BBCOORDINATESYSTEM2D_H
#define BBCOORDINATESYSTEM2D_H


#include "Base/BBGameObject.h"

class BBPositionCoordinateComponent2D;
class BBAABBBoundingBox2D;

class BBCoordinateSystem2D : public BBGameObject
{
protected:
    BBCoordinateSystem2D();
    ~BBCoordinateSystem2D();

    void setScreenCoordinate(int x, int y) override;
    void translate(int nDeltaX, int nDeltaY) override;
    void setScale(float scale, bool bUpdateLocalTransform = true) override;

    virtual bool hitAxis(int x, int y);
    virtual bool hitFace(int x, int y);
    virtual void transform(int x, int y) = 0;

    BBAxisFlags m_SelectedAxis;
    QPoint m_LastMousePos;
    BBGameObject *m_pSelectedObject;
    bool m_bTransforming;

    BBAABBBoundingBox2D *m_pBoundingBoxX;
    BBAABBBoundingBox2D *m_pBoundingBoxY;
    BBAABBBoundingBox2D *m_pBoundingBoxXOY;

public:
    void setSelectedObject(BBGameObject *pObject);
    virtual void setSelectedAxis(const BBAxisFlags &axis);
    bool mouseMoveEvent(int x, int y, bool bMousePressed);
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

    void setScreenCoordinate(int x, int y) override;
    void translate(int nDeltaX, int nDeltaY) override;

    void setSelectedAxis(const BBAxisFlags &axis) override;

private:
    void transform(int x, int y) override;

    BBPositionCoordinateComponent2D *m_pCoordinateComponent;
};

#endif // BBCOORDINATESYSTEM2D_H
