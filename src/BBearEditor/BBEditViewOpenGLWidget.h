#ifndef BBEDITVIEWOPENGLWIDGET_H
#define BBEDITVIEWOPENGLWIDGET_H

#include "BBOpenGLWidget.h"

class QMouseEvent;
class BBGameObject;
class BBGameObjectSet;
class BBEditViewOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    explicit BBEditViewOpenGLWidget(QWidget *pParent = 0);
    virtual ~BBEditViewOpenGLWidget();

//    BaseOpenGLWidget *getPreview();

private slots:
    void pressESC();
    void pressMoveKey(char key);
    void releaseMoveKey(char key);
    void pressTransform(char key);
    void setCoordinateSystemSelectedObject(BBGameObject *pGameObject);
    void setCoordinateSystemSelectedObjects(QList<BBGameObject*> gameObjects, BBGameObjectSet *pSet);
    void pressMultipleSelectionKey(bool bPressed);
    void updateCoordinateSystem();
    void createModelAtOrigin(const QString &filePath);
//    void lookAtGameObjectSlot(GameObject *gameObject);
//    void deleteSceneGameObject(GameObject *gameObject);
//    void createLightDependParent(QString fileName);
//    void copyGameObject(GameObject *sourceObject, QTreeWidgetItem* transcript, QVector3D position);
//    void onKeyPress(QKeyEvent *e);

signals:
    void addGameObject(BBGameObject *pGameObject);
    void pickObject(BBGameObject *pGameObject);
    void pickObjects(QList<BBGameObject*> gameObjects);
    void updateMultipleSelectedObjects(BBGameObject *pGameObject);
    void updateTransformInPropertyManager(BBGameObject *pGameObject, char transformModeKey);
//    void copyGameObjectInsertMap(QTreeWidgetItem *item, GameObject *gameObject);
//    void updateMaterialProperty(Model *model);

private:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    // Whether the right mouse button is pressed
    bool m_bRightPressed;
    QPoint m_OriginalMousePos;
    BBGameObject *m_pPreviewObject;

    QPoint m_SelectionRegionStartingPoint;
    bool m_bRegionSelecting;
    bool m_bMultipleSelecting;

//    void paintGL() override;
//    GLuint texture;
//    int deltaX;
//    int deltaY;
//    BaseOpenGLWidget *mPreview;
};

#endif // BBEDITVIEWOPENGLWIDGET_H
