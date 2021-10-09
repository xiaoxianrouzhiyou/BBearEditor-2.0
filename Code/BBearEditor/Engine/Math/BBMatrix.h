#ifndef BBMATRIX_H
#define BBMATRIX_H


class BBMatrix
{
public:
    static void SVDdecomposition(float w[3], float u[9], float v[9], float eps);

private:
    static inline float pythag(const float a, const float b);

    template<class T>
    static inline T sign(const T &a, const T &b)
    {
        return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
    }
};

#endif // BBMATRIX_H
