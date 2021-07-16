#include "BBStaticMesh.h"
#include "Utils/BBUtils.h"
#include "Render/BBVertexBufferObject.h"
#include <sstream>
#include "Geometry/BBBoundingBox.h"

BBOBJMesh::BBOBJMesh()
    : BBOBJMesh(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBOBJMesh::BBOBJMesh(float px, float py, float pz,
                     float rx, float ry, float rz,
                     float sx, float sy, float sz)
    : BBMesh(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBOBJMesh::load(const QString &path, QList<QVector4D> &outPositions)
{
    struct VertexDefine
    {
        int posIndex;
        int texcoordIndex;
        int normalIndex;
    };

    int fileSize = 0;
    char *pFileContent = BBUtils::loadFileContent(path.toStdString().c_str(), fileSize);
    if (pFileContent == NULL)
    {
        return;
    }

    // in the file, v... refers to vertex, f... refers to drawing instructions (How to organize the vertices)
    QList<QVector4D> positions, texcoords, normals;
    std::vector<VertexDefine> vertexes;
    std::vector<int> indexes;
    std::stringstream ssFileContent(pFileContent);
    std::string temp;
    char oneLine[256];
    while (!ssFileContent.eof())
    {
        memset(oneLine, 0, 256);
        ssFileContent.getline(oneLine, 256);
        // there are empty lines
        if (strlen(oneLine) > 0)
        {
            if (oneLine[0] == 'v')
            {
                // push a row of data into the stream
                // output from stream until space
                std::stringstream ssOneLine(oneLine);
                if (oneLine[1] == 't')
                {
                    // texcoord
                    ssOneLine >> temp; // vt...
                    float u, v;
                    // uv coordinate
                    ssOneLine >> u;
                    ssOneLine >> v;
                    // Put in the container
                    texcoords.append(QVector4D(u, v, 0, 0));
                }
                else if (oneLine[1] == 'n')
                {
                    // normal
                    ssOneLine >> temp;
                    float x, y, z;
                    ssOneLine >> x;
                    ssOneLine >> y;
                    ssOneLine >> z;
                    normals.append(QVector4D(x, y, z, 1.0));
                }
                else
                {
                    // vertex position
                    ssOneLine >> temp;
                    float x, y, z;
                    ssOneLine >> x;
                    ssOneLine >> y;
                    ssOneLine >> z;
                    positions.append(QVector4D(x, y, z, 1.0));
                }
            }
            else if (oneLine[0] == 'f')
            {
                // face, What points are made of each face
                std::stringstream ssOneLine(oneLine);
                ssOneLine >> temp; // f...
                std::string vertexStr;
                // Triangle, 3 pieces of data per line, separated by spaces
                for (int i = 0; i < 3; i++)
                {
                    ssOneLine >> vertexStr;
                    // The index value of pos and tex of each point, separated by /
                    size_t pos = vertexStr.find_first_of('/');
                    std::string posIndexStr = vertexStr.substr(0, pos);
                    size_t pos2 = vertexStr.find_first_of('/', pos + 1);
                    std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - pos - 1);
                    std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1);
                    VertexDefine vd;
                    // string to int
                    vd.posIndex = atoi(posIndexStr.c_str());
                    vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
                    vd.normalIndex = atoi(normalIndexStr.c_str());
                    // Check if it is a duplicate point. The same position, different normals, different points
                    int currentVertexIndex = -1;
                    int currentVertexCount = (int)vertexes.size();
                    for (int j = 0; j < currentVertexCount; j++)
                    {
                        if (vertexes[j].posIndex == vd.posIndex
                                && vertexes[j].normalIndex == vd.normalIndex
                                && vertexes[j].texcoordIndex == vd.texcoordIndex)
                        {
                            currentVertexIndex = j;
                            break;
                        }
                    }
                    // If the point does not exist, add the point container
                    if (currentVertexIndex == -1)
                    {
                        currentVertexIndex = (int)vertexes.size();
                        vertexes.push_back(vd);
                    }
                    // Regardless of whether it exists, record the index value of the point in the point container
                    indexes.push_back(currentVertexIndex);
                }
            }
        }
    }

    m_nIndexCount = (int)indexes.size();
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < m_nIndexCount; i++)
    {
        m_pIndexes[i] = indexes[i];
    }
    // How many unique points
    m_nVertexCount = (int)vertexes.size();
    m_pVBO = new BBVertexBufferObject(m_nVertexCount);
    for (int i = 0; i < m_nVertexCount; i++)
    {
        // vertexes[i].posIndex - 1
        // The index starts from 1 in .obj
        QVector4D temp;

        temp = positions.at(vertexes[i].posIndex - 1);
        m_pVBO->setPosition(i, temp);

        m_pVBO->setColor(i, m_DefaultColor);

        temp = texcoords.at(vertexes[i].texcoordIndex - 1);
        m_pVBO->setTexcoord(i, temp.x(), temp.y());

        temp = normals.at(vertexes[i].normalIndex - 1);
        m_pVBO->setNormal(i, temp);
    }

    // for creating bounding box
    outPositions = positions;

    BB_SAFE_DELETE(pFileContent);
}
