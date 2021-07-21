#include "BBPythonLoader.h"
#include <Python.h>
#include <QDebug>


void BBPythonLoader::runScript(const QString &path)
{
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        qDebug() << "init error";
        return;
    }
    qDebug() << "init ok";

    PyRun_SimpleString("import sys");
    QString sysPath = "sys.path.append('" + path + "')";
    PyRun_SimpleString(sysPath.toStdString().c_str());
    PyObject *pObj = PyImport_ImportModule("testPy");
    if (pObj == nullptr)
    {
        qDebug() << "PyImport_ImportModule error";
        return;
    }

    PyObject *pFunc = PyObject_GetAttrString(pObj, "printHello");
    if (pFunc == nullptr)
    {
        qDebug() << "func error";
        return;
    }

    PyEval_CallObject(pFunc, nullptr);
    Py_Finalize();
}
