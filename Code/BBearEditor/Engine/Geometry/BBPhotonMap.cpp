#include "BBPhotonMap.h"
#include "Math/BBMath.h"


BBPhotonMap::BBPhotonMap(int nMaxPhotonNum)
{
    m_nMaxPhotonNum = nMaxPhotonNum;
    m_nPhotonNum = 0;
    // For easy calculation, the index starts from 1
    m_pPhoton = new BBPhoton[nMaxPhotonNum + 1];
    m_BoxMin = QVector3D(1000000.0f, 1000000.0f, 1000000.0f);
    m_BoxMax = QVector3D(-1000000.0f, -1000000.0f, -1000000.0f);
}

BBPhotonMap::~BBPhotonMap()
{
    BB_SAFE_DELETE_ARRAY(m_pPhoton);
}

BBPhotonMap BBPhotonMap::operator=(const BBPhotonMap &photonMap)
{
    m_nPhotonNum = photonMap.getPhotonNum();
    m_nMaxPhotonNum = photonMap.getMaxPhotonNum();
    m_pPhoton = photonMap.getPhoton();
    m_BoxMin = photonMap.getBoxMin();
    m_BoxMax = photonMap.getBoxMax();
}

void BBPhotonMap::store(const BBPhoton &photon)
{
    BB_PROCESS_ERROR_RETURN((m_nPhotonNum <= m_nMaxPhotonNum));
    m_pPhoton[++m_nPhotonNum] = photon;
    m_BoxMin = QVector3D(std::min(m_BoxMin.x(), photon.m_Position.x()), std::min(m_BoxMin.y(), photon.m_Position.y()), std::min(m_BoxMin.z(), photon.m_Position.z()));
    m_BoxMax = QVector3D(std::max(m_BoxMax.x(), photon.m_Position.x()), std::max(m_BoxMax.y(), photon.m_Position.y()), std::max(m_BoxMax.z(), photon.m_Position.z()));
}

void BBPhotonMap::balance()
{
    // For easy calculation, the index starts from 1
    BBPhoton *pTmpPhoton = new BBPhoton[m_nPhotonNum + 1];
    for (int i = 1; i <= m_nPhotonNum; i++)
    {
        pTmpPhoton[i] = m_pPhoton[i];
    }
    balanceSegment(pTmpPhoton, 1, 1, m_nPhotonNum);
    BB_SAFE_DELETE_ARRAY(pTmpPhoton);
}

void BBPhotonMap::getKNearestPhotons(BBNearestPhotons *pNearestPhotons, int nParentIndex)
{
    if (nParentIndex > m_nPhotonNum)
        return;

    BBPhoton *pCurrentPhoton = &m_pPhoton[nParentIndex];
    //           1
    //       2       3
    //    4     5 6     7
    //  8   9
    // The index starts from 1, the index of the left subtree is 2 times
    if (nParentIndex * 2 <= m_nPhotonNum)
    {
        // There are sub nodes
        // The distance between detection photon and the dividing axis of current photon
        float d = pNearestPhotons->m_DetectionPosition[pCurrentPhoton->m_Axis] - pCurrentPhoton->m_Position[pCurrentPhoton->m_Axis];
        if (d < 0)
        {
            // The detection point is in the left of the dividing axis, search left subtree
            getKNearestPhotons(pNearestPhotons, nParentIndex * 2);
            if (d * d < pNearestPhotons->m_pDistanceSquare[0])
            {
                // search right subtree
                // When the distance is greater than detection scope, all nodes on the right subtree will be farther
                getKNearestPhotons(pNearestPhotons, nParentIndex * 2 + 1);
            }
        }
        else
        {
            // search right subtree
            getKNearestPhotons(pNearestPhotons, nParentIndex * 2 + 1);
            if (d * d < pNearestPhotons->m_pDistanceSquare[0])
            {
                // search left subtree
                getKNearestPhotons(pNearestPhotons, nParentIndex * 2);
            }
        }
    }
    // process itself
    float fDistanceSquare = (pNearestPhotons->m_DetectionPosition - pCurrentPhoton->m_Position).lengthSquared();
    if (fDistanceSquare > pNearestPhotons->m_pDistanceSquare[0])
        return;

    if (pNearestPhotons->m_nCurrentCount < pNearestPhotons->m_nMaxPhotonCount)
    {
        // When the heap is not full, qualified photons are inserted into the tail
        pNearestPhotons->m_nCurrentCount++;
        pNearestPhotons->m_pDistanceSquare[pNearestPhotons->m_nCurrentCount] = fDistanceSquare;
        pNearestPhotons->m_ppPhotons[pNearestPhotons->m_nCurrentCount] = pCurrentPhoton;
    }
    else
    {
        // When the heap is full
        if (!pNearestPhotons->m_bFulled)
        {
            // Just full, init maximum heap
            // Start from the parent node of the maximum sequence number to check whether the maximum heap is met. If not, adjust it
            // If yes, continue to check whether the previous parent node meets the requirements until node 1.
            for (int i = pNearestPhotons->m_nCurrentCount >> 1; i >= 1; i--)
            {
                int nParent = i;
                BBPhoton *pParentPhoton = pNearestPhotons->m_ppPhotons[nParent];
                float fParentParentPhoton = pNearestPhotons->m_pDistanceSquare[nParent];
                // Also check whether the adjusted node still meets the maximum heap property. If not, you need to traverse downward
                while ((nParent << 1) <= pNearestPhotons->m_nCurrentCount)
                {
                    // If parent has child
                    // nChild is left sub node
                    int nChild = nParent << 1;
                    // If the right sub node exists and is larger than the left sub node, nChild points to the right sub node
                    if (nChild + 1 <= pNearestPhotons->m_nCurrentCount && pNearestPhotons->m_pDistanceSquare[nChild] < pNearestPhotons->m_pDistanceSquare[nChild + 1])
                    {
                        nChild++;
                    }
                    // The larger sub node is compared with the parent node. If the parent node is large, it will not be processed.
                    if (fParentParentPhoton >= pNearestPhotons->m_pDistanceSquare[nChild])
                    {
                        break;
                    }
                    // Otherwise, the data of the larger sub node give the parent
                    pNearestPhotons->m_ppPhotons[nParent] = pNearestPhotons->m_ppPhotons[nChild];
                    pNearestPhotons->m_pDistanceSquare[nParent] = pNearestPhotons->m_pDistanceSquare[nChild];
                    // Traverse downward
                    nParent = nChild;
                }
                // Give remaining node data
                pNearestPhotons->m_ppPhotons[nParent] = pParentPhoton;
                pNearestPhotons->m_pDistanceSquare[nParent] = fParentParentPhoton;
            }
            // Change flag, When you insert data later, this code is no longer executed
            pNearestPhotons->m_bFulled = true;
        }
        // Insert new node in the maximum heap, Original node 1 (maximum) is removed, traverse downward
        int nParent = 1;
        while ((nParent << 1) <= pNearestPhotons->m_nCurrentCount)
        {
            int nChild = nParent << 1;
            if (nChild + 1 <= pNearestPhotons->m_nCurrentCount && pNearestPhotons->m_pDistanceSquare[nChild] < pNearestPhotons->m_pDistanceSquare[nChild + 1])
            {
                nChild++;
            }
            if (fDistanceSquare >= pNearestPhotons->m_pDistanceSquare[nChild])
            {
                break;
            }
            // Otherwise, the data of the larger sub node give the parent
            pNearestPhotons->m_ppPhotons[nParent] = pNearestPhotons->m_ppPhotons[nChild];
            pNearestPhotons->m_pDistanceSquare[nParent] = pNearestPhotons->m_pDistanceSquare[nChild];
            nParent = nChild;
        }
        pNearestPhotons->m_ppPhotons[nParent] = pCurrentPhoton;
        pNearestPhotons->m_pDistanceSquare[nParent] = fDistanceSquare;
        // [0] store the maximum
        pNearestPhotons->m_pDistanceSquare[0] = pNearestPhotons->m_pDistanceSquare[1];
    }
}

void BBPhotonMap::debug()
{
    for (int i = 1; i <= m_nPhotonNum; i++)
    {
        qDebug() << m_pPhoton[i].m_Position;
        qDebug() << m_pPhoton[i].m_Axis;
    }
}

void BBPhotonMap::debug(BBNearestPhotons *pNearestPhotons)
{
    for (int i = 1; i <= pNearestPhotons->m_nCurrentCount; i++)
    {
        qDebug() << pNearestPhotons->m_ppPhotons[i]->m_Position;
    }
}

void BBPhotonMap::markKNearestPhotons(BBNearestPhotons *pNearestPhotons)
{
    for (int i = 1; i <= pNearestPhotons->m_nCurrentCount; i++)
    {
        pNearestPhotons->m_ppPhotons[i]->m_bKNearestPhotons = true;
    }
}

bool BBPhotonMap::isMarkedKNearestPhotons(int nIndex)
{
    return m_pPhoton[nIndex].m_bKNearestPhotons;
}

QVector3D* BBPhotonMap::getPhotonPositions()
{
    QVector3D *pPositions = new QVector3D[m_nPhotonNum + 1];
    for (int i = 1; i <= m_nPhotonNum; i++)
    {
        pPositions[i] = m_pPhoton[i].m_Position;
    }
    return pPositions;
}

/**
 * @brief BBPhotonMap::splitMedian
 * @param pPhoton
 * @param start
 * @param end
 * @param median
 * @param axis                                                  0, 1, 2 : x, y, z
 */
void BBPhotonMap::splitMedian(BBPhoton pPhoton[], int start, int end, int median, int axis)
{
    int l = start;
    int r = end;
    while (l < r)
    {
        // sort, less than key, key, greater than key
        float key = pPhoton[r].m_Position[axis];
        int i = l - 1;
        int j = r;
        while (1)
        {
            while (pPhoton[++i].m_Position[axis] < key);
            while (pPhoton[--j].m_Position[axis] > key && j > l);
            if (i >= j)
                break;
            std::swap(pPhoton[i], pPhoton[j]);
        }
        std::swap(pPhoton[i], pPhoton[r]);

        // Make key in the middle
        if (i >= median)
            r = i - 1;
        if (i <= median)
            l = i + 1;
    }
}

void BBPhotonMap::balanceSegment(BBPhoton pPhoton[], int index, int start, int end)
{
    if (start == end)
    {
        m_pPhoton[index] = pPhoton[start];
        return;
    }

    int median = getMedian(start, end);
    // Select which axis to use for bifurcation
    int axis = 2;
    if ((m_BoxMax.x() - m_BoxMin.x() > m_BoxMax.y() - m_BoxMin.y()) && (m_BoxMax.x() - m_BoxMin.x() > m_BoxMax.z() - m_BoxMin.z()))
        axis = 0;
    else if (m_BoxMax.y() - m_BoxMin.y() > m_BoxMax.z() - m_BoxMin.z())
        axis = 1;

    // The median is calculated correctly, but the nodes on the left and right subtrees still need to be processed
    splitMedian(pPhoton, start, end, median, axis);
    m_pPhoton[index] = pPhoton[median];
    m_pPhoton[index].m_Axis = axis;

    if (start < median)
    {
        float tmp = m_BoxMax[axis];
        m_BoxMax[axis] = m_pPhoton[index].m_Position[axis];
        // index * 2 : Index of left subtree root
        balanceSegment(pPhoton, index * 2, start, median - 1);
        m_BoxMax[axis] = tmp;
    }
    if (end > median)
    {
        float tmp = m_BoxMin[axis];
        m_BoxMin[axis] = m_pPhoton[index].m_Position[axis];
        balanceSegment(pPhoton, index * 2 + 1, median + 1, end);
        m_BoxMin[axis] = tmp;
    }
}
