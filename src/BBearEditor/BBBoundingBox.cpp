#include "BBBoundingBox.h"
#include "BBUtils.h"
#include <cfloat>
#include "BBGLBuffers.h"
#include "BBGLShader.h"


//--------------------
// BBBoundingBox
//--------------------

BBBoundingBox::BBBoundingBox()
    : BBBoundingBox(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBBoundingBox::BBBoundingBox(const float &px, const float &py, const float &pz,
                             const float &rx, const float &ry, const float &rz,
                             const float &sx, const float &sy, const float &sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

BBBoundingBox::~BBBoundingBox()
{
    BB_SAFE_DELETE_ARRAY(m_Center);
}

bool BBBoundingBox::hitBoundingBox(BBRay ray, float &fDistance)
{
    Q_UNUSED(ray);
    Q_UNUSED(fDistance);
    return m_bActive;
}

QVector3D BBBoundingBox::getCenter()
{
    return QVector3D(m_Center[0], m_Center[1], m_Center[2]);
}


//--------------------
// BBBoundingBox3D
//--------------------

BBBoundingBox3D::BBBoundingBox3D(QList<QVector4D> vertexes)
    : BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, vertexes)
{

}

BBBoundingBox3D::BBBoundingBox3D(const float &px, const float &py, const float &pz,
                                 const float &rx, const float &ry, const float &rz,
                                 const float &sx, const float &sy, const float &sz,
                                 QList<QVector4D> vertexes)
{
    m_DefaultColor = QVector3D(0.909804f, 0.337255f, 0.333333f);
    m_Center[0] = 0;
    m_Center[1] = 0;
    m_Center[2] = 0;
    m_HalfLength[0] = 0.5f;
    m_HalfLength[1] = 0.5f;
    m_HalfLength[2] = 0.5f;
    m_Axis[0][0] = 1;
    m_Axis[0][1] = 0;
    m_Axis[0][2] = 0;
    m_Axis[1][0] = 0;
    m_Axis[1][1] = 1;
    m_Axis[1][2] = 0;
    m_Axis[2][0] = 0;
    m_Axis[2][1] = 0;
    m_Axis[2][2] = 1;
    // The constructor needs to be placed at the end
    // need to compute m_OriginalBoxVertexes at first
    computeOriginalBoxVertexes(vertexes);
    // otherwise, setModelMatrix() gets wrong m_TransformedBoxVertexes
    BBBoundingBox(px, py, pz, rx, ry, rz, sx, sy, sz);
}

BBBoundingBox3D::~BBBoundingBox3D()
{
    BB_SAFE_DELETE_ARRAY(m_OriginalBoxVertexes);
    BB_SAFE_DELETE_ARRAY(m_TransformedBoxVertexes);
    BB_SAFE_DELETE_ARRAY2D(m_Axis, 3);
    BB_SAFE_DELETE_ARRAY(m_HalfLength);
}

void BBBoundingBox3D::init()
{
    m_pVertexBuffer = new BBGLVertexBuffer(8);
    for (int i = 0; i < 8; i++)
    {
        m_pVertexBuffer->setPosition(i, m_TransformedBoxVertexes[i].x(), m_TransformedBoxVertexes[i].y(), m_TransformedBoxVertexes[i].z());
        m_pVertexBuffer->setColor(i, m_DefaultColor);
    }

    m_nIndexCount = 24;
    unsigned short indexes[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < m_nIndexCount; i++)
    {
        m_pIndexes[i] = indexes[i];
    }

    m_pShader->init(QString(BB_PATH_RESOURCE_SHADER) + "base.vert",
                    QString(BB_PATH_RESOURCE_SHADER) + "base.frag",
                    m_pIndexes,
                    m_nIndexCount);
}

bool BBBoundingBox3D::hitBoundingBox(BBRay ray, float &fDistance)
{
    // If it is not activated, no collision occurs
    if (!BBBoundingBox::hitBoundingBox(ray, fDistance))
        return false;

    QVector3D intersection;
    bool result = false;
    fDistance = FLT_MAX;
    // Calculate whether to hit the 6 faces of the bounding box
    static unsigned int indexes[] = {0, 1, 2, 3, 4, 5, 6, 7,
                                     1, 2, 6, 5, 0, 3, 7, 4,
                                     0, 1, 5, 4, 3, 2, 6, 7};
    for (int i = 0; i < 24; i += 4)
    {
        if (ray.computeIntersectWithRectangle(m_TransformedBoxVertexes[indexes[i]],
                                              m_TransformedBoxVertexes[indexes[i + 1]],
                                              m_TransformedBoxVertexes[indexes[i + 2]],
                                              m_TransformedBoxVertexes[indexes[i + 3]], intersection))
        {
            float temp = ray.computeIntersectDistance(intersection);
            if (temp < fDistance)
                fDistance = temp;
            result = true;
        }
    }
    return result;
}

void BBBoundingBox3D::setModelMatrix(const float px, const float py, const float pz,
                                     const QQuaternion r,
                                     const float sx, const float sy, const float sz)
{
    // When the corresponding model is transformed, invoke
    BBGameObject::setModelMatrix(px, py, pz, r, sx, sy, sz);
    for (int i = 0; i < 8; i++)
    {
        m_TransformedBoxVertexes[i] = m_ModelMatrix * m_OriginalBoxVertexes[i];
    }
}

void BBBoundingBox3D::draw()
{

}

void BBBoundingBox3D::computeOriginalBoxVertexes(QList<QVector4D> vertexes)
{
    FloatData temp[8];
    int sign[8][3] = {{1, 1, 1},
                      {-1, 1, 1},
                      {-1, -1, 1},
                      {1, -1, 1},
                      {1, 1, -1},
                      {-1, 1, -1},
                      {-1, -1, -1},
                      {1, -1, -1}};
    for (int index = 0; index < 8; index++)
    {
        for (int i = 0; i < 3; i++)
        {
            temp[index].v[i] = m_Center[i];
            for (int j = 0; j < 3; j++)
            {
                temp[index].v[i] += m_Axis[j][i] * m_HalfLength[j] * sign[index][j];
            }
        }
        m_OriginalBoxVertexes[index] = QVector3D(temp[index].v[0], temp[index].v[1], temp[index].v[2]);
    }
}


//#include <iostream>


///****************
// * RectBoundingBox2D
// *
// *****************/
//RectBoundingBox2D::RectBoundingBox2D(float centerX, float centerY, float centerZ,
//                                     float radiusX, float radiusY, float radiusZ)
//{
//    center[0] = centerX;
//    center[1] = centerY;
//    center[2] = centerZ;
//    if (radiusX == 0)
//    {
//        originBoxVertexes[0] = QVector3D(centerX, centerY + radiusY, centerZ + radiusZ);
//        originBoxVertexes[1] = QVector3D(centerX, centerY - radiusY, centerZ + radiusZ);
//        originBoxVertexes[2] = QVector3D(centerX, centerY - radiusY, centerZ - radiusZ);
//        originBoxVertexes[3] = QVector3D(centerX, centerY + radiusY, centerZ - radiusZ);
//    }
//    else if (radiusY == 0)
//    {
//        originBoxVertexes[0] = QVector3D(centerX + radiusX, centerY, centerZ + radiusZ);
//        originBoxVertexes[1] = QVector3D(centerX - radiusX, centerY, centerZ + radiusZ);
//        originBoxVertexes[2] = QVector3D(centerX - radiusX, centerY, centerZ - radiusZ);
//        originBoxVertexes[3] = QVector3D(centerX + radiusX, centerY, centerZ - radiusZ);
//    }
//    else if (radiusZ == 0)
//    {
//        originBoxVertexes[0] = QVector3D(centerX + radiusX, centerY + radiusY, centerZ);
//        originBoxVertexes[1] = QVector3D(centerX - radiusX, centerY + radiusY, centerZ);
//        originBoxVertexes[2] = QVector3D(centerX - radiusX, centerY - radiusY, centerZ);
//        originBoxVertexes[3] = QVector3D(centerX + radiusX, centerY - radiusY, centerZ);
//    }
//}

//void RectBoundingBox2D::transformBoundingBoxVertexes(QMatrix4x4 matrix)
//{
//    for (int i = 0; i < 4; i++)
//    {
//        transferedBoxVertexes[i] = matrix * originBoxVertexes[i];
//    }
//}

//bool RectBoundingBox2D::hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix)
//{
//    //如果没有激活 不产生碰撞
//    if (!BoundingBox::hitBoundingBox(ray, distance, matrix))
//        return false;
//    transformBoundingBoxVertexes(matrix);
//    QVector3D intersection;
//    bool result = false;
//    distance = FLT_MAX;
//    if (ray.computeIntersectWithRectangle(transferedBoxVertexes[0],
//                                          transferedBoxVertexes[1],
//                                          transferedBoxVertexes[2],
//                                          transferedBoxVertexes[3], intersection))
//    {
//        result = true;
//        distance = ray.computeIntersectDistance(intersection);
//    }
//    return result;
//}

///****************
// * QuarterRoundBoundingBox2D
// *
// *****************/

//QuarterRoundBoundingBox2D::QuarterRoundBoundingBox2D(float centerX, float centerY, float centerZ,
//                                       float radius, PlaneName plane)
//{
//    mOriginCenter = QVector3D(centerX, centerY, centerZ);
//    mPlane = plane;
//    if (plane == PlaneName::YOZ)
//    {
//        mOriginPoint = mOriginCenter + QVector3D(0, radius, 0);
//    }
//    else//plane == PlaneName::XOZ plane == PlaneName::XOY
//    {
//        mOriginPoint = mOriginCenter + QVector3D(radius, 0, 0);
//    }
//    setSign(1, 1, 1);
//}

//void QuarterRoundBoundingBox2D::transformBoundingBoxVertexes(QMatrix4x4 matrix)
//{
//    mTransferedCenter = matrix * mOriginCenter;
//    mTransferedPoint = matrix * mOriginPoint;
//}

//bool QuarterRoundBoundingBox2D::hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix)
//{
//    //如果没有激活 不产生碰撞
//    if (!BoundingBox::hitBoundingBox(ray, distance, matrix))
//        return false;
//    transformBoundingBoxVertexes(matrix);
//    QVector3D intersection;
//    bool result = false;
//    distance = FLT_MAX;
//    if (ray.computeIntersectWithQuarterRound(mTransferedCenter,
//                    (mTransferedPoint - mTransferedCenter).length(), mPlane, intersection, mXSign, mYSign, mZSign))
//    {
//        result = true;
//        distance = ray.computeIntersectDistance(intersection);
//    }
//    return result;
//}

//void QuarterRoundBoundingBox2D::setSign(int xSign, int ySign, int zSign)
//{
//    mXSign = xSign;
//    mYSign = ySign;
//    mZSign = zSign;
//}


///****************
// * TriangleBoundingBox2D
// *
// *****************/

//TriangleBoundingBox2D::TriangleBoundingBox2D(QVector3D point1, QVector3D point2, QVector3D point3)
//{
//    mOriginBoxVertexes[0] = point1;
//    mOriginBoxVertexes[1] = point2;
//    mOriginBoxVertexes[2] = point3;
//}

//void TriangleBoundingBox2D::transformBoundingBoxVertexes(QMatrix4x4 matrix)
//{
//    for (int i = 0; i < 3; i++)
//    {
//        mTransferedBoxVertexes[i] = matrix * mOriginBoxVertexes[i];
//    }
//}

//bool TriangleBoundingBox2D::hitBoundingBox(Ray ray, float &distance, QMatrix4x4 matrix)
//{
//    //如果没有激活 不产生碰撞
//    if (!BoundingBox::hitBoundingBox(ray, distance, matrix))
//        return false;
//    transformBoundingBoxVertexes(matrix);
//    QVector3D intersection;
//    bool result = false;
//    distance = FLT_MAX;
//    if (ray.computeIntersectWithTriangle(mTransferedBoxVertexes[0],
//                                         mTransferedBoxVertexes[1],
//                                         mTransferedBoxVertexes[2], intersection))
//    {
//        result = true;
//        distance = ray.computeIntersectDistance(intersection);
//    }
//    return result;
//}


///****************
// * BoundingBox3D
// *
// *****************/





//void BoundingBox3D::init(QList<QVector4D> vertexes)
//{
//    mVertexes = vertexes;
//    init();
//}

//void BoundingBox3D::draw()
//{
//    glEnable(GL_DEPTH_TEST);
//    glLineWidth(2);
//    glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, 0);
//}

//void BoundingBox3D::computeBoundingBox()
//{
//    getBoundingBoxVertexes();
//}

//QVector3D BoundingBox3D::getRadius()
//{
//    return QVector3D(radius[0], radius[1], radius[2]);
//}

//bool BoundingBox3D::belongToSelectionRegion(QVector3D left1, QVector3D left2, QVector3D left3,
//                                            QVector3D top1, QVector3D top2, QVector3D top3,
//                                            QVector3D right1, QVector3D right2, QVector3D right3,
//                                            QVector3D bottom1, QVector3D bottom2, QVector3D bottom3,
//                                            QMatrix4x4 matrix)
//{
//    //计算包围盒的每一个顶点是否在4个平面内测
//    transformBoundingBoxVertexes(matrix);
//    for (int i = 0; i < 8; i++)
//    {
//        if (transferedBoxVertexes[i].distanceToPlane(left1, left2, left3) < 0)
//            return false;
//        if (transferedBoxVertexes[i].distanceToPlane(top1, top2, top3) < 0)
//            return false;
//        if (transferedBoxVertexes[i].distanceToPlane(right1, right2, right3) < 0)
//            return false;
//        if (transferedBoxVertexes[i].distanceToPlane(bottom1, bottom2, bottom3) < 0)
//            return false;
//    }
//    return true;
//}


///****************
// * OBBBoundingBox3D
// *
// *****************/

//OBBBoundingBox3D::OBBBoundingBox3D()
//    : BoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1)
//{

//}

//OBBBoundingBox3D::OBBBoundingBox3D(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
//    : BoundingBox3D(px, py, pz, rx, ry, rz, sx, sy, sz)
//{

//}

//void OBBBoundingBox3D::computeBoundingBox()
//{
//    int vertexCount = mVertexes.count();

//    //初始化矩阵
//    MatrixXf input(3, vertexCount);
//    MatrixXf m(3, vertexCount);
//    for (int i = 0; i < vertexCount; i++)
//    {
//        m(0, i) = mVertexes.at(i).x();
//        m(1, i) = mVertexes.at(i).y();
//        m(2, i) = mVertexes.at(i).z();
//    }
//    input = m;
//    //cout << input << endl << endl;
//    //求取x y z均值
//    VectorXf avg = m.rowwise().mean();
//    m.colwise() -= avg;
//    //计算协方差矩阵 adjoint转置
//    MatrixXf covMat = (m * m.adjoint()) / float(vertexCount - 1);
//    //cout << covMat << endl << endl;
//    //求特征向量和特征值
//    SelfAdjointEigenSolver<MatrixXf> eigen(covMat);
//    MatrixXf eigenvectors = eigen.eigenvectors();
//    //cout << eigenvectors << endl << endl;
//    //将各点的（X，Y，Z）坐标投影到计算出的坐标轴上 avg由累加所有点再求均值得到 求出center和半长度
//    VectorXf minExtents(3);
//    VectorXf maxExtents(3);
//    minExtents << FLT_MAX, FLT_MAX, FLT_MAX;
//    maxExtents << -FLT_MAX, -FLT_MAX, -FLT_MAX;
//    for (int i = 0; i < vertexCount; i++)
//    {
//        VectorXf vertex = input.col(i);
//        VectorXf displacement = vertex - avg;
//        for (int j = 0; j < 3; j++)
//        {
//            minExtents[j] = min(minExtents[j], displacement.dot(eigenvectors.col(j)));
//            maxExtents[j] = max(maxExtents[j], displacement.dot(eigenvectors.col(j)));
//        }
//    }
//    VectorXf offset = (maxExtents - minExtents) / 2.0f + minExtents;
//    for (int i = 0; i < 3; i++)
//    {
//        avg += eigenvectors.col(i) * offset[i];
//    }

//    for (int i = 0; i < 3; i++)
//    {
//        center[i] = avg[i];
//        radius[i] = (maxExtents[i] - minExtents[i]) / 2.0f;
//        for (int j = 0; j < 3; j++)
//            axis[i][j] = eigenvectors.col(i)[j];
//    }
//    /*qDebug() << center[0] << center[1] << center[2];
//    qDebug() << radius[0] << radius[1] << radius[2];
//    qDebug() << axis[0][0] << axis[0][1] << axis[0][2];
//    qDebug() << axis[1][0] << axis[1][1] << axis[1][2];
//    qDebug() << axis[2][0] << axis[2][1] << axis[2][2];
//    qDebug() << endl;*/
//    getBoundingBoxVertexes();
//}

///****************
// * AABBBoundingBox3D
// *
// *****************/

//AABBBoundingBox3D::AABBBoundingBox3D()
//    : AABBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1)
//{

//}

//AABBBoundingBox3D::AABBBoundingBox3D(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
//    : BoundingBox3D(px, py, pz, rx, ry, rz, sx, sy, sz)
//{
//    for (int i = 0; i < 3; i++)
//    {
//        radius[i] = 0;
//    }
//    axis[0][0] = 1;
//    axis[0][1] = 0;
//    axis[0][2] = 0;
//    axis[1][0] = 0;
//    axis[1][1] = 1;
//    axis[1][2] = 0;
//    axis[2][0] = 0;
//    axis[2][1] = 0;
//    axis[2][2] = 1;
//}

//void AABBBoundingBox3D::computeBoundingBox()
//{
//    int vertexCount = mVertexes.count();

//    //初始化矩阵
//    MatrixXf m(3, vertexCount);
//    for (int i = 0; i < vertexCount; i++)
//    {
//        m(0, i) = mVertexes.at(i).x();
//        m(1, i) = mVertexes.at(i).y();
//        m(2, i) = mVertexes.at(i).z();
//    }
//    //cout << input << endl << endl;
//    //求取x y z均值 中心
//    VectorXf avg = m.rowwise().mean();
//    //计算各点各分量到中心的距离
//    m.colwise() -= avg;
//    for (int i = 0; i < 3; i++)
//    {
//        center[i] = avg[i];
//        for (int j = 0; j < vertexCount; j++)
//        {
//            radius[i] = max(radius[i], fabs(m(i, j)));
//        }
//    }
//    getBoundingBoxVertexes();
//}
