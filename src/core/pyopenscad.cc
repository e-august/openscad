#include <Python.h>
#include "pyopenscad.h"

static PyTypeObject PyOpenSCADType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyOpenSCAD",             /* tp_name */
    sizeof(PyOpenSCADObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "PyOpenSCAD objects",           /* tp_doc */
};

static PyModuleDef OpenScadModule = {
    PyModuleDef_HEAD_INIT,
    "PyOpenScadType",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_PyOpenSCAD(void)
{
    PyObject* m;

    PyOpenSCADType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyOpenSCADType) < 0)
        return NULL;

    m = PyModule_Create(&OpenScadModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyOpenSCADType);
    PyModule_AddObject(m, "PyOpenSCADObject", (PyObject *)&PyOpenSCADType);
    return m;
}

void PyOpenSCADObject_dealloc(PyOpenSCADObject * self)
{
    //Py_XDECREF(self->first);
    //Py_XDECREF(self->last);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject * PyOpenSCADObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyOpenSCADObject *self;

    self = (PyOpenSCADObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        //self->number = 0;
    }

    return (PyObject *)self;
}

