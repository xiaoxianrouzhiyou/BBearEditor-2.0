#include "BBSSAOGlobalIllumination.h"
#include <QImage>
#include "Math/BBMath.h"
#include <QDebug>


float* BBSSAOGlobalIllumination::generateKernel()
{
    // range : 0.0~1.0
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    float *pKernel = new float[64 * 4];;
    for (unsigned int i = 0; i < 64; i++)
    {
        QVector3D sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
        sample.normalize();
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        pKernel[i * 4] = sample.x();
        pKernel[i * 4 + 1] = sample.y();
        pKernel[i * 4 + 2] = sample.z();
        pKernel[i * 4 + 3] = 1.0f;
    }
    return pKernel;
}

float* BBSSAOGlobalIllumination::generateNoise()
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    float *pNoise = new float[16 * 4];
    for (unsigned int i = 0; i < 16; i++)
    {
        // rotate around z-axis (in tangent space)
        pNoise[i * 4] = randomFloats(generator) * 2.0f - 1.0f;
        pNoise[i * 4 + 1] = randomFloats(generator) * 2.0f - 1.0f;
        pNoise[i * 4 + 2] = 0.0f;
        pNoise[i * 4 + 3] = 1.0f;
    }
    return pNoise;
}
