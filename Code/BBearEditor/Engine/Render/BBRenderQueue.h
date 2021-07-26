#ifndef BBRENDERQUEUE_H
#define BBRENDERQUEUE_H


class BBDrawCall;
class BBCamera;
class BBMaterial;

class BBRenderQueue
{
public:
    BBRenderQueue(BBCamera *pCamera);
    ~BBRenderQueue();

    void appendOpaqueDrawCall(BBDrawCall *pDC);
    void appendTransparentDrawCall(BBDrawCall *pDC);
    void appendUIDrawCall(BBDrawCall *pDC);

    void removeOpaqueDrawCall(BBDrawCall *pDC);
    void removeTransparentDrawCall(BBDrawCall *pDC);
    void removeUIDrawCall(BBDrawCall *pDC);

    void render();
    void renderOpaque();
    void renderTransparent();
    void renderUI();

    void updateAllDrawCallOrder();

    void updateOpaqueDrawCallOrder(BBDrawCall *pNode);
    void updateTransparentDrawCallOrder(BBDrawCall *pNode);

    void switchQueue(BBMaterial *pPrevious, BBMaterial *pCurrent, BBDrawCall *pDrawCall);

private:
    void updateAllOpaqueDrawCallOrder();
    void updateAllTransparentDrawCallOrder();

    BBDrawCall* appendAscendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode);
    BBDrawCall* appendDescendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode);

    BBCamera *m_pCamera;
    BBDrawCall *m_pOpaqueDrawCall;
    BBDrawCall *m_pTransparentDrawCall;
    BBDrawCall *m_pUIDrawCall;
};

#endif // BBRENDERQUEUE_H
