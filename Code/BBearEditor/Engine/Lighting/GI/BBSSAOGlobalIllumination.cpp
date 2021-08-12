#include "BBSSAOGlobalIllumination.h"
#include <QImage>
#include "Math/BBMath.h"


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

QImage BBSSAOGlobalIllumination::generateNoise()
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    QImage noiseTex(4, 4, QImage::Format_RGB32);
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            // rotate around z-axis (in tangent space)
            QVector3D noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f);
            QColor color;
            color.setRgbF(noise.x(), noise.y(), noise.z());
            noiseTex.setPixelColor(i, j, color);
        }
    }
    return noiseTex;
}
