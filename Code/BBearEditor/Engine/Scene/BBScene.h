#ifndef BBSCENE_H
#define BBSCENE_H


#include <QList>
#include <QVector3D>
#include <Serializer/BBGameObject.pb.h>


class BBFrameBufferObject;
class BBCamera;
class BBSkyBox;
class BBHorizontalPlane;
class BBGameObject;
class BBModel;
class BBLight;
class BBSelectionRegion;
class BBRay;
class BBTransformCoordinateSystem;
class BBFullScreenQuad;
class BBScene
{
public:
    BBScene();
    virtual ~BBScene();

    void init();
    void render();
    void resize(float width, float height);

    inline BBCamera* getCamera() { return m_pCamera; }
    inline BBTransformCoordinateSystem* getTransformCoordinateSystem() { return m_pTransformCoordinateSystem; }
    inline QList<BBGameObject*> getLights() { return m_Lights; }

    /* FBO */
    inline BBFrameBufferObject* getFBO() { return m_pFBO; }
    void enableFBO(bool bEnable) { m_bEnableFBO = bEnable; }
    void enableFullScreenQuad(bool bEnable) { m_bEnableFullScreenQuad = bEnable; }

    void setSkyBox(const QString &path);
    void enableSkyBox(bool bEnable) { m_bEnableSkyBox = bEnable; }
    void enableHorizontalPlane(bool bEnable) { m_bHorizontalPlane = bEnable; }

    BBModel* createModel(const QString &filePath, int x, int y);
    BBModel* createModel(const QString &filePath,
                         const QVector3D &position = QVector3D(0, 0, 0),
                         const QVector3D &rotation = QVector3D(0, 0, 0),
                         const QVector3D &scale = QVector3D(1, 1, 1));
    BBModel* createModel(const BBSerializer::BBGameObject &gameObject);
    BBModel* createModelForPreview(const QString &filePath, float fDistFactor = 2.0f);

    BBLight* createLight(const QString &fileName, int x, int y, bool bSelect = true);
    BBLight* createLight(const QString &fileName,
                         const QVector3D &position = QVector3D(0, 0, 0),
                         const QVector3D &rotation = QVector3D(0, 0, 0),
                         bool bSelect = true);

    BBGameObject* pickObject(const BBRay &ray, bool bSelect = true);
    void lookAtGameObject(BBGameObject *pGameObject);
    void deleteGameObject(BBGameObject *pGameObject);

    void setSelectionRegionVisibility(bool bVisible);
    QList<BBGameObject*> getSelectedObjects(QPoint start, QPoint end);

    void clear();

private:
    void bindFBO();
    void unbindFBO();
    BBFrameBufferObject *m_pFBO;
    bool m_bEnableFBO;
    static QString m_ColorBufferName;

private:
    float m_fUpdateRate;

    BBCamera *m_pCamera;
    BBSkyBox *m_pSkyBox;
    bool m_bEnableSkyBox;
    BBHorizontalPlane *m_pHorizontalPlane;
    bool m_bHorizontalPlane;
    BBTransformCoordinateSystem *m_pTransformCoordinateSystem;
    BBFullScreenQuad *m_pFullScreenQuad;
    bool m_bEnableFullScreenQuad;

    QList<BBGameObject*> m_Models;
    QList<BBGameObject*> m_Lights;

    BBSelectionRegion *m_pSelectionRegion;

//    QList<GameObject*> audios;

//    void renderShadowMap();
//    Audio *createAudio(QString filePath, int x, int y, bool isSelect = true);
//    Audio *createAudio(QString filePath, QVector3D position = QVector3D(0, 0, 0), bool isSelect = true);
//    bool setModelMaterial(Model *model, QString mtlPath);
//    void play();
//    void stop();
//    Sprite2D sprite;
//    GLuint particleTexture;
};

#endif // BBSCENE_H
