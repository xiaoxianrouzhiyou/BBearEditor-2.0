#include "BBPythonLoader.h"
#include <Python.h>
#include "Utils/BBUtils.h"
#include <exception>


void BBPythonLoader::runScript(const QString &path)
{
    // set home path for python
    Py_SetPythonHome(L"./");

    Py_Initialize();

    try
    {
        PyRun_SimpleString("print('Hello World!')");
//        FILE *pFile = fopen(path.toStdString().c_str(), "r");
//        if (!pFile)
//        {
//            throw 0;
//        }
//        PyRun_AnyFile(pFile, path.toStdString().c_str());

        Py_Finalize();
    }
    catch (int)
    {
        Py_Finalize();
    }


}
