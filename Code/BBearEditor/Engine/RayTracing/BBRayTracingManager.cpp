#include "BBRayTracingManager.h"
#include "Utils/BBUtils.h"
#include "Scene/BBSceneManager.h"


void BBRayTracingManager::open()
{
    // Stop refresh per frame
    BBSceneManager::getEditViewOpenGLWidget();
}

void BBRayTracingManager::close()
{

}
