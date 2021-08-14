#ifndef BBPYTHONLOADER_H
#define BBPYTHONLOADER_H


#include <Python.h>
#include <QString>

class BBPythonLoader
{
public:
    static void runScript(const QString &path);

private:
    static PyObject *m_pModule;
};

#endif // BBPYTHONLOADER_H
