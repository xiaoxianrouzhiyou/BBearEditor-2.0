#include "BBOBJMesh.h"
#include "BBUtils.h"
#include "BBGLBuffers.h"
#include <sstream>

BBOBJMesh::BBOBJMesh()
    : BBOBJMesh(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBOBJMesh::BBOBJMesh(const float px, const float py, const float pz,
                     const float rx, const float ry, const float rz,
                     const float sx, const float sy, const float sz)
    : BBMesh(px, py, pz, rx, ry, rz, sx, sy, sz)
{

}

void BBOBJMesh::init(const QString path)
{
    QString suffix = path.mid(path.lastIndexOf('.') + 1);
    if (suffix == "obj")
    {
        load(path);
        BBMesh::init(path);
    }
}

void BBOBJMesh::load(const QString path)
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
    m_pVertexBuffer = new BBGLVertexBuffer(m_nVertexCount);
    for (int i = 0; i < m_nVertexCount; i++)
    {
        // vertexes[i].posIndex - 1
        // The index starts from 1 in .obj
        QVector4D temp;

        temp = positions.at(vertexes[i].posIndex - 1);
        m_pVertexBuffer->setPosition(i, temp);

        m_pVertexBuffer->setColor(i, m_Color);

        temp = texcoords.at(vertexes[i].texcoordIndex - 1);
        m_pVertexBuffer->setTexcoord(i, temp.x(), temp.y());

        temp = normals.at(vertexes[i].normalIndex - 1);
        m_pVertexBuffer->setNormal(i, temp);
    }

//    //计算包围盒的数据
//    mVertexPositions = positions;

    BB_SAFE_DELETE(pFileContent);
}
