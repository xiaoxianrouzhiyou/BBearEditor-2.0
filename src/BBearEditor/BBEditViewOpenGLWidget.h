#ifndef BBEDITVIEWOPENGLWIDGET_H
#define BBEDITVIEWOPENGLWIDGET_H

#include "BBOpenGLWidget.h"

class QMouseEvent;
class BBGameObject;
class BBEditViewOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    explicit BBEditViewOpenGLWidget(QWidget *pParent = 0);
    virtual ~BBEditViewOpenGLWidget();

//    BaseOpenGLWidget *getPreview();

public slots:
    void pressESCSlot();

private slots:
    void pressMoveKeySlot(char key);
    void releaseMoveKeySlot(char key);
    void pressTransformSlot(char key);
//    void multipleSelectKey(bool isPress);
//    void lookAtGameObjectSlot(GameObject *gameObject);
//    void setCoordinateSelectedObject(GameObject *gameObject);
//    void setCoordinateSelectedObjects(QList<GameObject*> gameObjects, CenterPoint *center);
//    void deleteSceneGameObject(GameObject *gameObject);
//    void updateCoordinate();
//    void createModelDependParent(QString filePath);
//    void createLightDependParent(QString fileName);
//    void copyGameObject(GameObject *sourceObject, QTreeWidgetItem* transcript, QVector3D position);
//    void onKeyPress(QKeyEvent *e);

signals:
    void addGameObjectSignal(BBGameObject *pGameObject);
//    void updateTransform(GameObject *gameObject, char transformModeKey);
//    void pickObject(GameObject *gameObject);
//    void updateMultipleSelectObjects(GameObject *gameObject);
//    void regionSelectObjects(QList<GameObject*> gameObjects);
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

//    void paintGL() override;
//    GLuint texture;
//    int deltaX;
//    int deltaY;

//    bool isMultipleSelect;
//    QPoint selectionRegionStart;
//    bool isRegionSelecting;
//    BaseOpenGLWidget *mPreview;
};

#endif // BBEDITVIEWOPENGLWIDGET_H
