#ifndef BBPYTHONVM_H
#define BBPYTHONVM_H


#include <Python.h>
#include <QString>

class BBPythonVM
{
public:
    static void runScript(const QString &path);

private:
    static bool loadMainModule();
    static bool loadDictionary();
    static bool loadClass();
    static bool loadFunc();

    static bool emitValues();
    static bool emitFunc();

    static PyObject* cfunc(PyObject *pSelf, PyObject *pArgs);

private:
    static PyObject *m_pModule;
};

#endif // BBPYTHONVM_H
