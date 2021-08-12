#include "BBSSAOGlobalIllumination.h"
#include <QImage>
#include "Math/BBMath.h"
#include <QDebug>


std::vector<QVector3D> BBSSAOGlobalIllumination::generateKernel()
{
    // range : 0.0~1.0
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    std::vector<QVector3D> ssaoKernel;
    for (unsigned int i = 0; i < 64; i++)
    {
        QVector3D sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
        sample.normalize();
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    return ssaoKernel;
}

float* BBSSAOGlobalIllumination::generateNoise()
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    float *pPixelData = new float[16 * 4];
    for (unsigned int i = 0; i < 16; i++)
    {
        // rotate around z-axis (in tangent space)
        pPixelData[i * 4] = randomFloats(generator) * 2.0f - 1.0f;
        pPixelData[i * 4 + 1] = randomFloats(generator) * 2.0f - 1.0f;
        pPixelData[i * 4 + 2] = 0.0f;
        pPixelData[i * 4 + 3] = 1.0f;
    }
    return pPixelData;
}
