#ifndef BBEDITVIEWOPENGLWIDGET_H
#define BBEDITVIEWOPENGLWIDGET_H

#include "BBOpenGLWidget.h"
#include <QVector3D>
#include "Serializer/BBGameObject.pb.h"


class QMouseEvent;
class BBGameObject;
class BBGameObjectSet;
class QTreeWidgetItem;
class BBEditViewOpenGLWidget : public BBOpenGLWidget
{
    Q_OBJECT

public:
    explicit BBEditViewOpenGLWidget(QWidget *pParent = 0);
    virtual ~BBEditViewOpenGLWidget();

    BBGameObject* createModel(const BBSerializer::BBGameObject &gameObject);

private:
    void setRenderThread();
    QThread *m_pRenderThread;
    QTimer *m_pRenderTimer;

private slots:
    void pressESC();
    void pressMoveKey(char key);
    void releaseMoveKey(char key);
    void pressTransform(char key);
    void setCoordinateSystemSelectedObject(BBGameObject *pGameObject);
    void setCoordinateSystemSelectedObjects(const QList<BBGameObject*> &gameObjects, BBGameObjectSet *pSet);
    void pressMultipleSelectionKey(bool bPressed);
    void updateCoordinateSystem();
    void createModelAtOrigin(const QString &filePath);
    void createLightAtOrigin(const QString &fileName);
    void deleteGameObject(BBGameObject *pGameObject);
    void copyGameObject(BBGameObject *pSourceObject, QTreeWidgetItem *pTranscriptItem);
    void lookAtGameObject(BBGameObject *pGameObject);

signals:
    void updateEditViewTitle();
    // create QTreeWidgetItem
    void addGameObject(BBGameObject *pGameObject);
    // specified QTreeWidgetItem
    void addGameObject(BBGameObject *pGameObject, QTreeWidgetItem *pItem);
    void pickObject(BBGameObject *pGameObject);
    void pickObjects(QList<BBGameObject*> gameObjects);
    void updateMultipleSelectedObjects(BBGameObject *pGameObject);
    void updateTransformInPropertyManager(BBGameObject *pGameObject, char transformModeKey);

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
};

#endif // BBEDITVIEWOPENGLWIDGET_H
