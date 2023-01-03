#define NPY_NO_DEPRECATED_API NPY_API_VERSION
#include "numpy/arrayobject.h"
#include "vector_add.h"
#include <Python.h>

static unsigned long
cfib(unsigned long n) {
  unsigned long a = 1;
  unsigned long b = 1;
  unsigned long c;

  if (n <= 1) {
    return 1;
  }

  while (--n > 1) {
    c = a + b;
    a = b;
    b = c;
  }

  return b;
}

static PyObject *
py_fib(PyObject *self, PyObject *n) {
  unsigned long as_unsigned_long = PyLong_AsUnsignedLong(n);
  unsigned long fib_result = cfib(as_unsigned_long);
  return PyLong_FromUnsignedLong(fib_result);
}

PyDoc_STRVAR(fib_doc, "computes the nth Fibonacci number");

PyDoc_STRVAR(vector_add_doc, "element-wise adds two vectors");

static PyObject *
py_vector_add(PyObject *self, PyObject *n) {
  Py_RETURN_NONE;
}

PyMethodDef methods[] = {
    {"fib", (PyCFunction)py_fib, METH_O, fib_doc},
    {"vector_add", (PyCFunction)py_vector_add, METH_O, vector_add_doc},
    {NULL},
};

PyDoc_STRVAR(fib_module_doc, "provides a Fibonacci function");

PyModuleDef fib_module = {PyModuleDef_HEAD_INIT,
                          "fib",
                          fib_module_doc,
                          -1,
                          methods,
                          NULL,
                          NULL,
                          NULL,
                          NULL};

PyMODINIT_FUNC
PyInit_fib(void) {
  import_array();
  if (PyErr_Occurred())
    return NULL;

  return PyModule_Create(&fib_module);
}
