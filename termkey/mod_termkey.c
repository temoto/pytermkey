#include <Python.h>
#include <stdio.h>
#include "structmember.h"
#include <termkey.h>

#include "mod_termkey.h"

static PyObject *module;
static PyTypeObject KeyWrapType; // forward decl


// *************************************
// common begins
// *************************************

#ifndef NDEBUG
#define DPRINT(s)              printf("debug: " s)
#define DPRINT1(s, arg0)       printf("debug: " s, arg0)
#define DPRINT2(s, arg0, arg1) printf("debug: " s, arg0, arg1)
#define DPRINT(s)
#define DPRINT1(s, arg0)
#define DPRINT2(s, arg0, arg1)
#else
#define DPRINT(s)
#define DPRINT1(s, arg0)
#define DPRINT2(s, arg0, arg1)
#endif

// *************************************
// common ends
// *************************************


// *************************************
// TermKeyWrap begins
// *************************************

PyDoc_STRVAR(TermKeyWrap_doc,
"TODO");

static int
TermKeyWrap_init(TermKeyWrap *self, PyObject *args) {
    int fd = 0, flags = 0;

    if (!PyArg_ParseTuple(args, "ii", &fd, &flags)) {
        PyErr_SetString(PyExc_TypeError, "TermKey() takes two int arguments: file descriptor and flags expected. Pass two zeros if unsure.");
        return -1;
    }

    self->tk = termkey_new(fd, flags);
    self->fd = fd;

    return 0;
}

static void
TermKeyWrap_dealloc(TermKeyWrap *self) {
    if (NULL != self->tk) {
        termkey_destroy(self->tk);
        self->tk = NULL;
        self->fd = -1;
    }
    self->ob_type->tp_free((PyObject*)self);
}

/*@null@*/
static PyObject*
TermKeyWrap_close(TermKeyWrap *self, PyObject *args) {
    if (args && !PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "TermKey.close() takes no arguments.");
        return NULL;
    }

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return NULL;
    }

    termkey_destroy(self->tk);
    self->tk = NULL;
    self->fd = -1;

    Py_RETURN_NONE;
}

/*@null@*/
static PyObject*
TermKeyWrap_getkey(TermKeyWrap *self, PyObject *args) {
    TermKeyResult result;
    TermKeyKey key;
    KeyWrap *keywrap;

    if (args && !PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "TermKey.getkey() takes no arguments.");
        return NULL;
    }

    assert(NULL != self->tk);

    memset(&key, 0, sizeof(key));

    result = termkey_getkey(self->tk, &key);

    keywrap = (KeyWrap *)PyObject_CallObject((PyObject*)&KeyWrapType, NULL);
    keywrap->tk = self->tk;
    keywrap->tk_key = key;

    return Py_BuildValue("(iN)", result, keywrap);
}

/*@null@*/
static PyObject*
TermKeyWrap_getkey_force(TermKeyWrap *self, PyObject *args) {
    TermKeyResult result;
    TermKeyKey key;
    KeyWrap *keywrap;

    if (args && !PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "TermKey.getkey() takes no arguments.");
        return NULL;
    }

    assert(NULL != self->tk);

    memset(&key, 0, sizeof(key));

    result = termkey_getkey_force(self->tk, &key);

    keywrap = (KeyWrap *)PyObject_CallObject((PyObject*)&KeyWrapType, NULL);
    keywrap->tk = self->tk;
    keywrap->tk_key = key;

    return Py_BuildValue("(iN)", result, keywrap);
}

/*@null@*/
static PyObject*
TermKeyWrap_waitkey(TermKeyWrap *self, PyObject *args) {
    TermKeyResult result;
    TermKeyKey key;
    KeyWrap *keywrap;

    if (args && !PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "TermKey.getkey() takes no arguments.");
        return NULL;
    }

    assert(NULL != self->tk);

    memset(&key, 0, sizeof(key));

    result = termkey_waitkey(self->tk, &key);

    keywrap = (KeyWrap *)PyObject_CallObject((PyObject*)&KeyWrapType, NULL);
    keywrap->tk = self->tk;
    keywrap->tk_key = key;

    return Py_BuildValue("(iN)", result, keywrap);
}

static PyMethodDef TermKeyWrap_methods[] = {
    {"close", (PyCFunction)TermKeyWrap_close, METH_NOARGS,
     "TODO"},
    {"getkey", (PyCFunction)TermKeyWrap_getkey, METH_NOARGS,
     "TODO"},
    {"getkey_force", (PyCFunction)TermKeyWrap_getkey_force, METH_NOARGS,
     "TODO"},
    {"waitkey", (PyCFunction)TermKeyWrap_waitkey, METH_NOARGS,
     "TODO"},
    {NULL} /* Sentinel */
};

static PyMemberDef TermKeyWrap_members[] = {
    {"fd", T_INT, offsetof(TermKeyWrap, fd), READONLY, "TODO"},
    {NULL} /* Sentinel */
};

/*@null@*/
static PyObject*
TermKeyWrap_get_flags(TermKeyWrap *self, void *closure) {
    int flags = 0;

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return NULL;
    }

    flags = termkey_get_flags(self->tk);

    return Py_BuildValue("i", flags);
}

static int
TermKeyWrap_set_flags(TermKeyWrap *self, PyObject *value, void *closure) {
    int flags = 0;
    long l;

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return -1;
    }

    l = PyInt_AsLong(value);
    if (-1 == l && NULL != PyErr_Occurred())
        return -1;

    flags = (int)l;

    termkey_set_flags(self->tk, flags);

    return 0;
}

/*@null@*/
static PyObject*
TermKeyWrap_get_waittime(TermKeyWrap *self, void *closure) {
    int waittime = 0;

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return NULL;
    }

    waittime = termkey_get_waittime(self->tk);

    return Py_BuildValue("i", waittime);
}

static int
TermKeyWrap_set_waittime(TermKeyWrap *self, PyObject *value, void *closure) {
    int waittime = 0;
    long l;

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return -1;
    }

    l = PyInt_AsLong(value);
    if (-1 == l && NULL != PyErr_Occurred())
        return -1;

    waittime = (int)l;

    termkey_set_waittime(self->tk, waittime);

    return 0;
}

static PyGetSetDef TermKeyWrap_getseters[] = {
    {"flags", (getter)TermKeyWrap_get_flags, (setter)TermKeyWrap_set_flags,
        "TODO",
        NULL},
    {"waittime", (getter)TermKeyWrap_get_waittime, (setter)TermKeyWrap_set_waittime,
        "TODO",
        NULL},
    {NULL} /* Sentinel */
};

/* TermKeyWrapType */
static PyTypeObject TermKeyWrapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_termkey.TermKey",                       /*tp_name*/
    sizeof(TermKeyWrap),                      /*tp_basicsize*/
    0,                                        /*tp_itemsize*/
    (destructor)TermKeyWrap_dealloc,          /*tp_dealloc*/
    0,                                        /*tp_print*/
    0,                                        /*tp_getattr*/
    0,                                        /*tp_setattr*/
    0,                                        /*tp_compare*/
    0,                                        /*tp_repr*/
    0,                                        /*tp_as_number*/
    0,                                        /*tp_as_sequence*/
    0,                                        /*tp_as_mapping*/
    0,                                        /*tp_hash */
    0,                                        /*tp_call*/
    0,                                        /*tp_str*/
    0,                                        /*tp_getattro*/
    0,                                        /*tp_setattro*/
    0,                                        /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    TermKeyWrap_doc,                          /*tp_doc*/
    0,                                        /*tp_traverse*/
    0,                                        /*tp_clear*/
    0,                                        /*tp_richcompare*/
    0,                                        /*tp_weaklistoffset*/
    0,                                        /*tp_iter*/
    0,                                        /*tp_iternext*/
    TermKeyWrap_methods,                      /*tp_methods*/
    TermKeyWrap_members,                      /*tp_members*/
    TermKeyWrap_getseters,                    /*tp_getsets*/
    0,                                        /*tp_base*/
    0,                                        /*tp_dict*/
    0,                                        /*tp_descr_get*/
    0,                                        /*tp_descr_set*/
    offsetof(TermKeyWrap, __dict__),          /*tp_dictoffset*/
    (initproc)TermKeyWrap_init,               /*tp_init*/
};

// *************************************
// TermKeyWrap ends
// *************************************


// *************************************
// KeyWrap begins
// *************************************

PyDoc_STRVAR(KeyWrap_doc,
"TermKey Key object");

static PyMemberDef KeyWrap_members[] = {
    {"type_", T_INT, offsetof(KeyWrap, tk_key.type), READONLY,
     "TODO"},
    {"codepoint", T_LONG, offsetof(KeyWrap, tk_key.code.codepoint), READONLY,
     "TODO"},
    {"number", T_INT, offsetof(KeyWrap, tk_key.code.number), READONLY,
     "TODO"},
    {"sym", T_INT, offsetof(KeyWrap, tk_key.code.sym), READONLY,
     "TODO"},
    {"modifiers", T_INT, offsetof(KeyWrap, tk_key.modifiers), READONLY,
     "TODO"},
    //{"utf8", T_STRING, offsetof(KeyWrap, tk_key.utf8), READONLY,
    // "TODO"},
    {NULL} /* Sentinel */
};

/*@null@*/
static PyObject*
KeyWrap_str(KeyWrap *self) {
#define KEYWRAP_STR_BUF_SIZE 1000
    static char buf[KEYWRAP_STR_BUF_SIZE];
    size_t r = 0;

    if (NULL == self->tk) {
        PyErr_SetString(PyExc_ValueError, "TermKey already closed or not yet initialized.");
        return NULL;
    }

    memset(buf, 0, KEYWRAP_STR_BUF_SIZE);
    r = termkey_snprint_key(self->tk, buf, KEYWRAP_STR_BUF_SIZE - 1,
                            &(self->tk_key),
                            TERMKEY_FORMAT_LONGMOD);

    return Py_BuildValue("s", buf);
}

/* KeyWrapType */
static PyTypeObject KeyWrapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_termkey.Key",                           /*tp_name*/
    sizeof(KeyWrap),                          /*tp_basicsize*/
    0,                                        /*tp_itemsize*/
    0,                                        /*tp_dealloc*/
    0,                                        /*tp_print*/
    0,                                        /*tp_getattr*/
    0,                                        /*tp_setattr*/
    0,                                        /*tp_compare*/
    0,                                        /*tp_repr*/
    0,                                        /*tp_as_number*/
    0,                                        /*tp_as_sequence*/
    0,                                        /*tp_as_mapping*/
    0,                                        /*tp_hash */
    0,                                        /*tp_call*/
    (reprfunc)KeyWrap_str,                    /*tp_str*/
    0,                                        /*tp_getattro*/
    0,                                        /*tp_setattro*/
    0,                                        /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    KeyWrap_doc,                              /*tp_doc*/
    0,                                        /*tp_traverse*/
    0,                                        /*tp_clear*/
    0,                                        /*tp_richcompare*/
    0,                                        /*tp_weaklistoffset*/
    0,                                        /*tp_iter*/
    0,                                        /*tp_iternext*/
    0,                                        /*tp_methods*/
    KeyWrap_members,                          /*tp_members*/
    0,                                        /*tp_getsets*/
    0,                                        /*tp_base*/
    0,                                        /*tp_dict*/
    0,                                        /*tp_descr_get*/
    0,                                        /*tp_descr_set*/
    offsetof(KeyWrap, __dict__),              /*tp_dictoffset*/
};

// *************************************
// KeyWrap ends
// *************************************


// *************************************
// module begins
// *************************************

static PyMethodDef module_methods[] = {
    {NULL, NULL, 0, NULL} /* Sentinel */
};

PyMODINIT_FUNC
init_termkey(void) {
    // init types
    TermKeyWrapType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&TermKeyWrapType) < 0)
        return;
    KeyWrapType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&KeyWrapType) < 0)
        return;

    // init module
    module = Py_InitModule("_termkey", module_methods);
    if (NULL == module)
        return;

    PyModule_AddIntConstant(module, "RES_NONE",  TERMKEY_RES_NONE);
    PyModule_AddIntConstant(module, "RES_KEY",   TERMKEY_RES_KEY);
    PyModule_AddIntConstant(module, "RES_EOF",   TERMKEY_RES_EOF);
    PyModule_AddIntConstant(module, "RES_AGAIN", TERMKEY_RES_AGAIN);

    PyModule_AddIntConstant(module, "FLAG_NOINTERPRET", TERMKEY_FLAG_NOINTERPRET);
    PyModule_AddIntConstant(module, "FLAG_CONVERTKP", TERMKEY_FLAG_CONVERTKP);
    PyModule_AddIntConstant(module, "FLAG_RAW", TERMKEY_FLAG_RAW);
    PyModule_AddIntConstant(module, "FLAG_UTF8", TERMKEY_FLAG_UTF8);
    PyModule_AddIntConstant(module, "FLAG_NOTERMIOS", TERMKEY_FLAG_NOTERMIOS);

    Py_INCREF(&TermKeyWrapType);
    PyModule_AddObject(module, "TermKey", (PyObject*)&TermKeyWrapType);
    Py_INCREF(&KeyWrapType);
    PyModule_AddObject(module, "Key", (PyObject*)&KeyWrapType);
}

// *************************************
// module ends
// *************************************

