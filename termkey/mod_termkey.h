#ifndef mod_termkey_h
#define mod_termkey_h

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <stdbool.h>

#include <termkey.h>


typedef struct {
    PyObject_HEAD
    PyObject *__dict__;
    TermKey *tk;
    int fd;
} TermKeyWrap;

typedef struct {
    PyObject_HEAD
    PyObject *__dict__;
    TermKey *tk;
    TermKeyKey tk_key;
} KeyWrap;


#ifdef __cplusplus
}
#endif
#endif // mod_termkey_h
