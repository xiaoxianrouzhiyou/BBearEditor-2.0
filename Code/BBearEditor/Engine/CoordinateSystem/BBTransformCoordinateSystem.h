#ifndef BBTRANSFORMCOORDINATESYSTEM_H
#define BBTRANSFORMCOORDINATESYSTEM_H


#include "Base/BBGameObject.h"

class BBGameObjectSet;
class BBPositionCoordinateSystem;
class BBRotationCoordinateSystem;
class BBScaleCoordinateSystem;
class BBPositionCoordinateSystem2D;
class BBRotationCoordinateSystem2D;
class BBScaleCoordinateSystem2D;

// Class that manages the three Coordinate system
class BBTransformCoordinateSystem : public BBGameObject
{
public:
    BBTransformCoordinateSystem();
    virtual ~BBTransformCoordinateSystem();

    void init() override;
    void render(BBCamera *pCamera) override;

    void setSelectedObject(BBGameObject *pObject);
    void setSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet);
    bool mouseMoveEvent(int x, int y, const BBRay &ray, bool bMousePressed);
    void setCoordinateSystem(char modeKey);
    bool isTransforming() { return m_bTransforming; }
    void stopTransform();
    void update();
    inline BBGameObject* getSelectedObject() { return m_pSelectedObject; }
    inline char getTransformModeKey() { return m_ModeKey; }

private:
    void switchSpaceMode(const BBCoordinateSystemSpaceMode &eMode);

    BBCoordinateSystemSpaceMode m_eSpaceMode;

    BBPositionCoordinateSystem *m_pPositionCoordinateSystem;
    BBRotationCoordinateSystem *m_pRotationCoordinateSystem;
    BBScaleCoordinateSystem *m_pScaleCoordinateSystem;

    BBPositionCoordinateSystem2D *m_pPositionCoordinateSystem2D;
    BBRotationCoordinateSystem2D *m_pRotationCoordinateSystem2D;
    BBScaleCoordinateSystem2D *m_pScaleCoordinateSystem2D;

    char m_PositionCoordinateSystemModeKey = 'W';
    char m_RotationCoordinateSystemModeKey = 'E';
    char m_ScaleCoordinateSystemModeKey = 'R';
    char m_ModeKey;

    BBGameObject *m_pSelectedObject;
    QList<BBGameObject*> m_SelectedObjects;
    bool m_bTransforming;
};

#endif // BBTRANSFORMCOORDINATESYSTEM_H
