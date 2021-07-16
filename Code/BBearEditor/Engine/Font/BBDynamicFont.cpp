#include "BBDynamicFont.h"


#define INIT_CHAR_ID(size, style, outlineSize, shadowX, shadowY, charCode) \
uint64 charID; \
do { \
    char *pStyle = (char*)&charID; \
    *(pStyle++) = size + style; \
    *(pStyle++) = outlineSize; \
    *(pStyle++) = shadowX; \
    *(pStyle++) = shadowY; \
    *((unsigned int*)pStyle) = charCode; \
} while(0)


BBDynamicFont::BBDynamicFont()
{

}
