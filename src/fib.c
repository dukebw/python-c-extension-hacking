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
py_fib(PyObject *module, PyObject *n) {
  unsigned long as_unsigned_long = PyLong_AsUnsignedLong(n);
  unsigned long fib_result = cfib(as_unsigned_long);
  return PyLong_FromUnsignedLong(fib_result);
}

PyDoc_STRVAR(fib_doc, "computes the nth Fibonacci number");

PyDoc_STRVAR(vector_add_doc, "element-wise adds two vectors");

static PyObject *
py_vector_add(PyObject *module, PyObject *args) {
  PyObject *a_obj;
  PyObject *b_obj;
  PyObject *c_obj;
  if (!PyArg_UnpackTuple(args, "vector_add", 3, 3, &c_obj, &a_obj, &b_obj))
    return NULL;

  PyArrayObject *c_np = (PyArrayObject *)PyArray_FROM_OTF(
      c_obj, NPY_FLOAT, (NPY_ARRAY_INOUT_ARRAY2 | NPY_ARRAY_ENSUREARRAY));
  if (c_np == NULL)
    return NULL;

  PyArrayObject *a_np = (PyArrayObject *)PyArray_FROM_OTF(
      a_obj, NPY_FLOAT, (NPY_ARRAY_IN_ARRAY | NPY_ARRAY_ENSUREARRAY));
  if (a_np == NULL)
    goto cleanup;

  PyArrayObject *b_np = (PyArrayObject *)PyArray_FROM_OTF(
      b_obj, NPY_FLOAT, (NPY_ARRAY_IN_ARRAY | NPY_ARRAY_ENSUREARRAY));
  if (b_np == NULL)
    goto cleanup;

  float *c = (float *)PyArray_DATA(c_np);
  Py_ssize_t c_nelem = PyArray_SIZE(c_np);

  const float *a = (const float *)PyArray_DATA(a_np);
  Py_ssize_t a_nelem = PyArray_SIZE(a_np);

  const float *b = (const float *)PyArray_DATA(b_np);
  Py_ssize_t b_nelem = PyArray_SIZE(b_np);

  assert((a_nelem == b_nelem) && (a_nelem == c_nelem));

  vector_add(c, a, b, a_nelem);

  Py_DECREF(b_np);
  Py_DECREF(a_np);
  Py_DECREF(c_np);

  Py_RETURN_NONE;

cleanup:
  Py_XDECREF(b_np);
  Py_XDECREF(a_np);
  PyArray_DiscardWritebackIfCopy(c_np);
  Py_DECREF(c_np);

  return NULL;
}

PyMethodDef methods[] = {
    {"fib", (PyCFunction)py_fib, METH_O, fib_doc},
    {"vector_add", (PyCFunction)py_vector_add, METH_VARARGS, vector_add_doc},
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
