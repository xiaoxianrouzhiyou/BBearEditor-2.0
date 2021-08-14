#include "BBPythonLoader.h"
#include "Utils/BBUtils.h"


PyObject* BBPythonLoader::m_pModule = nullptr;

void BBPythonLoader::runScript(const QString &path)
{
    bool bResult = false;

    do
    {
        // set home path for python
        Py_SetPythonHome(L"../../Code/BBearEditor/Engine/Python");
        Py_Initialize();

        // PyRun_SimpleString("print('Hello World!')");
        PyObject *pObj = Py_BuildValue("s", path.toStdString().c_str());
        FILE *pFile = _Py_fopen_obj(pObj, "r+");
        BB_PROCESS_ERROR(pFile);
        PyRun_SimpleFile(pFile, path.toStdString().c_str());

//        m_pModule = PyImport_ImportModule("PyModule");
//        BB_PROCESS_ERROR(m_pModule);

        bResult = true;
    } while(0);

    if (!bResult)
    {
        qDebug() << "error";
        PyErr_Print();
    }

    Py_Finalize();
}
