#ifndef BBSSAOGLOBALILLUMINATION_H
#define BBSSAOGLOBALILLUMINATION_H


#include <vector>
#include <QVector3D>

class BBSSAOGlobalIllumination
{
public:
    static std::vector<QVector3D> generateKernel();
    static float* generateNoise();
};

#endif // BBSSAOGLOBALILLUMINATION_H
