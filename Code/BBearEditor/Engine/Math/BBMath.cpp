#include <stdlib.h>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


/**
 * @brief frandom
 * @return                  0~1
 */
float frandom()
{
    return rand() / (float)RAND_MAX;
}


/**
 * @brief sfrandom
 * @return                  -1~1
 */
float sfrandom()
{
    return frandom() * 2.0f - 1.0f;
}
