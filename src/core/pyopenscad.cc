// Author: Sohler Guenther
// Date: 2023-01-01
// Purpose: Extend openscad with an python interpreter
#include <Python.h>
#include "pyopenscad.h"

// https://docs.python.it/html/ext/dnt-basics.html

std::shared_ptr<AbstractNode> result_node=NULL;
std::vector<std::shared_ptr<AbstractNode>> node_stack;



/* Return the number of arguments of the application command line */

static PyMethodDef OpenSCADMethods[] = {
    {"cube", (PyCFunction) openscad_cube, METH_VARARGS | METH_KEYWORDS, "Create Cube."},
    {"cylinder", (PyCFunction) openscad_cylinder, METH_VARARGS | METH_KEYWORDS, "Create Cylinder."},
    {"sphere", (PyCFunction) openscad_sphere, METH_VARARGS | METH_KEYWORDS, "Create Sphere."},

    {"translate", (PyCFunction) openscad_translate, METH_VARARGS | METH_KEYWORDS, "Move  Object."},
    {"rotate", (PyCFunction) openscad_rotate, METH_VARARGS | METH_KEYWORDS, "Rotate Object."},

    {"union", (PyCFunction) openscad_union, METH_VARARGS | METH_KEYWORDS, "Union Object."},
    {"difference", (PyCFunction) openscad_difference, METH_VARARGS | METH_KEYWORDS, "Difference Object."},
    {"intersection", (PyCFunction) openscad_intersection, METH_VARARGS | METH_KEYWORDS, "Intersection Object."},

    {"output", (PyCFunction) openscad_output, METH_VARARGS | METH_KEYWORDS, "Output the result."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef OpenSCADModule = {
    PyModuleDef_HEAD_INIT, "openscad", NULL, -1, OpenSCADMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_openscad(void)
{
    return PyModule_Create(&OpenSCADModule);
}




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

PyObject * PyOpenSCADObject_new( std::shared_ptr<AbstractNode> node)
{
    PyOpenSCADObject *self;
	printf("a\n");
    self = (PyOpenSCADObject *)malloc(sizeof(PyOpenSCADObject));
    printf("b self=%p %d\n",self,sizeof(PyOpenSCADObject));
    if (self != NULL) {
	self->node=node; // node;
    }
	printf("C\n");
    printf("Self=%p\n",self);
    printf("b\n");
    return (PyObject *)self;
}


void evaluatePython(const char *code)
{
    result_node=NULL;
    wchar_t *program = Py_DecodeLocale("openscad", NULL);
    const char *prg="from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n";
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
//    Py_SetProgramName(program);  /* optional but recommended /

    PyImport_AppendInittab("openscad", &PyInit_openscad);

    Py_Initialize();
    PyInit_PyOpenSCAD();
    PyRun_SimpleString("from openscad import *\n");
    PyRun_SimpleString(code);
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
}

