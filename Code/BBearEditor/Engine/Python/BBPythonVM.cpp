#include "BBPythonVM.h"
#include "Utils/BBUtils.h"
#include <iostream>


PyObject* BBPythonVM::m_pModule = nullptr;

void BBPythonVM::runScript(const QString &path)
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

        BB_PROCESS_ERROR(emitValues());
        PyRun_SimpleFile(pFile, path.toStdString().c_str());

        loadMainModule();
        BB_PROCESS_ERROR(m_pModule);
        BB_PROCESS_ERROR(emitFunc());

//        BB_PROCESS_ERROR(loadDictionary());
//        BB_PROCESS_ERROR(loadClass());
//        BB_PROCESS_ERROR(loadFunc());

        bResult = true;
    } while(0);

    if (!bResult)
    {
        qDebug() << "error";
        PyErr_Print();
    }

    Py_XDECREF(m_pModule);
    Py_Finalize();
}

bool BBPythonVM::loadMainModule()
{
    PyObject *pKey = PyUnicode_FromString("__main__");
    m_pModule = PyImport_GetModule(pKey);
    // clear
    Py_XDECREF(pKey);
    return true;
}

bool BBPythonVM::loadDictionary()
{
    // get object by module and name
    PyObject *pDict = PyObject_GetAttrString(m_pModule, "config");
    BB_PROCESS_ERROR_RETURN_FALSE(pDict);

    PyObject *pKey = PyUnicode_FromString("width");
    int nWidth = PyLong_AsLong(PyDict_GetItem(pDict, pKey));
    Py_XDECREF(pKey);

    pKey = PyUnicode_FromString("height");
    int nHeight = PyLong_AsLong(PyDict_GetItem(pDict, pKey));
    Py_XDECREF(pKey);

    pKey = PyUnicode_FromString("title");
    wchar_t title[1024] = { 0 };
    int nSize = PyUnicode_AsWideChar(PyDict_GetItem(pDict, pKey), title, 1023);
    Py_XDECREF(pKey);

    qDebug() << "w" << nWidth << "h" << nHeight << "title" << title[0];

    Py_XDECREF(pDict);
    return true;
}

bool BBPythonVM::loadClass()
{
    // get class
    PyObject *pPyClass = PyObject_GetAttrString(m_pModule, "BBPythonBase");
    BB_PROCESS_ERROR_RETURN_FALSE(pPyClass);

    // Instantiate object, and invoke __init__, and pass params to constructor (there is no parameter here)
    PyObject *pObj = PyObject_CallObject(pPyClass, nullptr);
    BB_PROCESS_ERROR_RETURN_FALSE(pObj);

    // invoke member func
    // params: i(int), s(str)
    PyObject *pRet = PyObject_CallMethod(pObj, "func0", "is", 2021, "Bear");
    qDebug() << PyLong_AsLong(pRet);

    // Member variable
    PyObject *pVar = PyObject_GetAttrString(pObj, "id");
    qDebug() << PyLong_AsLong(pVar);

    Py_XDECREF(pVar);
    Py_XDECREF(pRet);
    Py_XDECREF(pObj);
    Py_XDECREF(pPyClass);
    return true;
}

bool BBPythonVM::loadFunc()
{
    PyObject *pFunc0 = PyObject_GetAttrString(m_pModule, "main");
    if (pFunc0 && PyCallable_Check(pFunc0))
    {
        // func obj + params
        PyObject_CallObject(pFunc0, 0);
    }

    PyObject *pFunc1 = PyObject_GetAttrString(m_pModule, "func1");
    if (pFunc1 && PyCallable_Check(pFunc1))
    {
        // param list, count = 1
        PyObject *pArgs = PyTuple_New(1);
        PyObject *pList = PyList_New(0);
        for (int i = 0; i < 5; i++)
        {
            PyList_Append(pList, PyLong_FromLong(i + 100));
        }
        PyTuple_SetItem(pArgs, 0, pList);
        PyObject *pRet = PyObject_CallObject(pFunc1, pArgs);
        if (pRet)
        {
            int nSize = PyList_Size(pRet);
            for (int i = 0; i < nSize; i++)
            {
                PyObject *pVar = PyList_GetItem(pRet, i);
                if (!pVar)
                    continue;
                qDebug() << PyLong_AsLong(pVar);
            }
            Py_XDECREF(pRet);
        }

        // pList has been passed into pArgs
        Py_XDECREF(pArgs);
    }

    Py_XDECREF(pFunc0);
    Py_XDECREF(pFunc1);
    return true;
}

bool BBPythonVM::emitValues()
{
    PyRun_SimpleString("Bear = 100");
    return true;
}

bool BBPythonVM::emitFunc()
{
    PyMethodDef cfuncs[] = {
        {"cfunc", cfunc, METH_VARARGS, 0},
        {NULL}
    };
    PyModule_AddFunctions(m_pModule, cfuncs);
    return true;
}

PyObject* BBPythonVM::cfunc(PyObject *pSelf, PyObject *pArgs)
{
    std::cout << "cfunc" << endl;
//    Py_RETURN_TRUE;
}
