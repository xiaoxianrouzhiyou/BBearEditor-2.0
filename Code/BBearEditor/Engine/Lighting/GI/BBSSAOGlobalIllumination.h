#ifndef BBSSAOGLOBALILLUMINATION_H
#define BBSSAOGLOBALILLUMINATION_H


#include <vector>
#include <QVector3D>

class BBSSAOGlobalIllumination
{
public:
    static float *generateKernel();
    static float *generateNoise();
};

#endif // BBSSAOGLOBALILLUMINATION_H
