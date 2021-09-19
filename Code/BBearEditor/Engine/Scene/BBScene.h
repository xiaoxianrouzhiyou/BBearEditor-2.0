#ifndef BBSCENE_H
#define BBSCENE_H


#include <QList>
#include <QVector3D>
#include <Serializer/BBGameObject.pb.h>
#include "Render/BBBaseRenderComponent.h"

class BBOpenGLWidget;
class BBFrameBufferObject;
class BBVertexBufferObject;
class BBCamera;
class BBSkyBox;
class BBHorizontalPlane;
class BBGameObject;
class BBModel;
class BBLight;
class BBCanvas;
class BBSpriteObject2D;
class BBParticleSystem;
class BBSelectionRegion;
class BBRay;
class BBTransformCoordinateSystem;
class BBFullScreenQuad;
class BBTiledFullScreenQuad;
class BBRenderQueue;
class BBNormalIndicator;
class BBScene;

typedef void (BBScene::*BBRenderingFunc)();

class BBScene
{
public:
    BBScene(BBOpenGLWidget *pOpenGLWidget);
    virtual ~BBScene();

    void init();

    void render();
    void setRenderingFunc(const BBRenderingFunc &renderingFunc) { m_RenderingFunc = renderingFunc; }
    void defaultRendering();
    // the rendering times Objects_ScreenQuad
    void deferredRendering1_1();
    void deferredRendering1_2();
    void deferredRendering2_1();

    void resize(float width, float height);

    inline BBRenderQueue* getRenderQueue() { return m_pRenderQueue; }
    inline BBCamera* getCamera() { return m_pCamera; }
    inline BBSkyBox* getSkyBox() { return m_pSkyBox; }
    inline BBTransformCoordinateSystem* getTransformCoordinateSystem() { return m_pTransformCoordinateSystem; }
    inline BBFullScreenQuad* getFullScreenQuad(int nIndex) { return m_pFullScreenQuad[nIndex]; }
    inline BBTiledFullScreenQuad* getTiledFullScreenQuad() { return m_pTiledFullScreenQuad; }
    inline QList<BBGameObject*> getModels() { return m_Models; }
    inline QList<BBGameObject*> getLights() { return m_Lights; }
    inline BBNormalIndicator* getNormalIndicator() { return m_pNormalIndicator; }
    inline GLuint getShadowMapMipmap() { return m_ShadowMap; }

    /* FBO */
    GLuint getColorFBO(int nFBOIndex, int nAttachmentIndex = 0);
    GLuint getDepthFBO(int nFBOIndex);

    /* SkyBox */
    void setSkyBox(const QString &path);
    void changeSkyBoxAlgorithm(int nAlgorithmIndex);
    void enableSkyBox(bool bEnable);

    void enableHorizontalPlane(bool bEnable);

    BBModel* createModel(const QString &userData, int x, int y);
    BBModel* createModel(const QString &userData,
                         const QVector3D &position = QVector3D(0, 0, 0), const QVector3D &rotation = QVector3D(0, 0, 0), const QVector3D &scale = QVector3D(1, 1, 1));
    BBModel* createModel(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount,
                         const QVector3D &position = QVector3D(0, 0, 0), const QVector3D &rotation = QVector3D(0, 0, 0), const QVector3D &scale = QVector3D(1, 1, 1));
    BBModel* createModel(const BBSerializer::BBGameObject &gameObject);
    BBModel* createModelForPreview(const QString &filePath, float fDistFactor = 2.0f);

    BBLight* createLight(const QString &fileName, int x, int y, bool bSelect = true);
    BBLight* createLight(const QString &fileName, const QVector3D &position = QVector3D(0, 0, 0), const QVector3D &rotation = QVector3D(0, 0, 0), bool bSelect = true);

    BBCanvas* createCanvas(int x, int y, bool bSelect = true);

    BBSpriteObject2D* createSpriteObject2D(BBCanvas *pCanvas, int x, int y, bool bSelect = true);

    BBParticleSystem* createParticleSystem(int x, int y, bool bSelect = true);

    bool hitCanvas(int x, int y, BBCanvas *&pOutCanvas);
    BBGameObject* pickObject(const QList<BBGameObject*> &alternativeObjects, const BBRay &ray, bool bSelect = true);
    BBGameObject* pickObjectInAllObjects(const BBRay &ray, bool bSelect = true);
    BBGameObject* pickObjectInModels(const BBRay &ray, bool bSelect = true);

    void lookAtGameObject(BBGameObject *pGameObject);
    void deleteGameObject(BBGameObject *pGameObject);

    void setSelectionRegionVisibility(bool bVisible);
    QList<BBGameObject*> getSelectedObjects(QPoint start, QPoint end);

    void clear();

    void update();

public:
    void setFullScreenQuadTexture(const std::string &uniformName, GLuint textureName);

private:
    void bindFBO();
    void unbindFBO();
    void writeSkyBoxCubeMap();
    void writeViewSpaceFBO(int nIndex);
    void writeShadowMap();

    BBFrameBufferObject *m_pFixedSizeFBO;
    BBFrameBufferObject *m_pFBO[3];

    GLuint m_ShadowMap;
    BBFrameBufferObject *m_pShadowMapFBO;

private:
    BBOpenGLWidget *m_pOpenGLWidget;

    BBRenderingFunc m_RenderingFunc;
    BBRenderQueue *m_pRenderQueue;

    float m_fUpdateRate;

    BBCamera *m_pCamera;
    BBSkyBox *m_pSkyBox;
    BBHorizontalPlane *m_pHorizontalPlane;
    BBTransformCoordinateSystem *m_pTransformCoordinateSystem;
    BBFullScreenQuad *m_pFullScreenQuad[3];
    BBTiledFullScreenQuad *m_pTiledFullScreenQuad;

    QList<BBGameObject*> m_Models;
    QList<BBGameObject*> m_Lights;
    QList<BBGameObject*> m_Canvases;
    QList<BBGameObject*> m_ParticleSystems;

    BBSelectionRegion *m_pSelectionRegion;

    BBNormalIndicator *m_pNormalIndicator;
};

#endif // BBSCENE_H
