#include <Python.h>
#include <stdio.h>


#define CHECK_NULL(v) do {                                                    \
        if (NULL == v) {                                                      \
            PyErr_Print();                                                    \
            goto cleanup;                                                     \
        }                                                                     \
    } while(0)


int main(void) {
    int rv = 1;
    PyObject *module = NULL, *termkey_class = NULL, *termkey_obj = NULL, *p = NULL;

    Py_Initialize();

    module = PyImport_ImportModule("termkey");
    CHECK_NULL(module);

    termkey_class = PyObject_GetAttrString(module, "TermKey");
    CHECK_NULL(termkey_class);
printf("0\n");
    termkey_obj = PyObject_CallFunction(termkey_class, "ii", 0, 0);
    CHECK_NULL(termkey_obj);
printf("1\n");
    // get a key
    p = PyObject_CallMethod(termkey_obj, "getkey", NULL);
    CHECK_NULL(p);
    Py_DECREF(p); /* not XDECREF */
printf("2\n");

    p = PyObject_GetAttrString(termkey_obj, "fd");
    CHECK_NULL(p);
    Py_DECREF(p); /* not XDECREF */

    rv = 0;

cleanup:
    Py_XDECREF(termkey_obj);
    Py_XDECREF(termkey_class);
    Py_XDECREF(module);

    Py_Finalize();
    return rv;
}
