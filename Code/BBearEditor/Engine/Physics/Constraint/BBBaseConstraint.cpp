#include "BBBaseConstraint.h"
#include "../Body/BBBaseBody.h"
#include "Utils/BBUtils.h"


BBBaseConstraint::BBBaseConstraint(BBBaseBody *pBody)
{
    m_pBody = pBody;
}

BBBaseConstraint::~BBBaseConstraint()
{
    BB_SAFE_DELETE(m_pBody);
}
