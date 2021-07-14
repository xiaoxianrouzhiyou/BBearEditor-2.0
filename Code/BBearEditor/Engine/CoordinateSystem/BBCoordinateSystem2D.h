#ifndef BBCOORDINATESYSTEM2D_H
#define BBCOORDINATESYSTEM2D_H


#include "Base/BBGameObject.h"

class BBCoordinateComponent2D;
class BBBoundingBox2D;
class BBCoordinateCircle2D;
class BBCoordinateTickMark2D;
class BBCoordinateSector2D;

class BBCoordinateSystem2D : public BBGameObject
{
public:
    BBCoordinateSystem2D();
    ~BBCoordinateSystem2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setScreenCoordinate(int x, int y) override;
    void translate(int nDeltaX, int nDeltaY) override;
    void setScale(float scale, bool bUpdateLocalTransform = true) override;

    bool hitAxis(int x, int y);
    bool hitFace(int x, int y);
    virtual void transform(int x, int y) = 0;

    void setSelectedObject(BBGameObject *pObject);
    virtual void setSelectedAxis(const BBAxisFlags &axis);
    virtual bool mouseMoveEvent(int x, int y, bool bMousePressed);
    virtual void stopTransform();

protected:
    BBAxisFlags m_SelectedAxis;
    QPoint m_LastMousePos;
    BBGameObject *m_pSelectedObject;
    bool m_bTransforming;

    BBCoordinateComponent2D *m_pCoordinateComponent;
    BBBoundingBox2D *m_pBoundingBoxX;
    BBBoundingBox2D *m_pBoundingBoxY;
    BBBoundingBox2D *m_pBoundingBoxXOY;
};


class BBPositionCoordinateSystem2D : public BBCoordinateSystem2D
{
public:
    BBPositionCoordinateSystem2D();

private:
    void transform(int x, int y) override;
};


class BBRotationCoordinateSystem2D : public BBCoordinateSystem2D
{
public:
    BBRotationCoordinateSystem2D();
    ~BBRotationCoordinateSystem2D();

    void init() override;
    void render(BBCamera *pCamera) override;
    void resize(float fWidth, float fHeight) override;

    void setScreenCoordinate(int x, int y) override;

    void stopTransform() override;

private:
    void transform(int x, int y) override;

    BBCoordinateCircle2D *m_pCoordinateCircle;
    BBCoordinateTickMark2D *m_pCoordinateTickMark;
    BBCoordinateSector2D *m_pCoordinateSector;
};


class BBScaleCoordinateSystem2D : public BBCoordinateSystem2D
{
public:
    BBScaleCoordinateSystem2D();

    void setRotation(const QVector3D &rotation, bool bUpdateLocalTransform = true) override;
    bool mouseMoveEvent(int x, int y, bool bMousePressed) override;
    void stopTransform() override;

private:
    void transform(int x, int y) override;

    // current - start
    int m_nDeltaX;
    int m_nDeltaY;
};

#endif // BBCOORDINATESYSTEM2D_H
