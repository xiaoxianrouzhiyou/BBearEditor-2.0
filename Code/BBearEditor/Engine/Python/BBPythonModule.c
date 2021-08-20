#include "Python.h"


// module function
static PyObject *BBearModuleFunc(PyObject *pSelf)
{
    // create py long, python to release, reference count +1 in C
    return PyLong_FromLong(101);
}

// module function list
static PyMethodDef BBearModule_Func[] =
{
    {
        "BBearModule Func",                 // function name
        BBearModuleFunc,                    // function pointer
        METH_NOARGS,                        // module flag: no parameter
        "BBearModule Func instruction"      // function instruction, use help(func name) to get
    },
    {0, 0, 0, 0}
};

static PyModuleDef moduleDef =
{
    PyModuleDef_HEAD_INIT,
    "BBearModule",                          // module name
    "BBearModule instruction",              // module instruction, use help(module name) to get
    -1,                                     // module space, using by sub interpreter, we do not use it for the time
    BBearModule_Func                        // module function
};

// Extension library entry function
// PyInit_ fixed prefix
PyMODINIT_FUNC PyInit_BBearModule(void)
{
    // create module
    return PyModule_Create(&moduleDef);
}
