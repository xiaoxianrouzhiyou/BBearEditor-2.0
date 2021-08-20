#ifndef BBPYTHONVM_H
#define BBPYTHONVM_H


#include <Python.h>
#include <QString>

class BBPythonLoader
{
public:
    static void runScript(const QString &path);

private:
    static PyObject *m_pModule;
};

#endif // BBPYTHONVM_H
