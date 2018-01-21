//
// Copyright 2017 Alsanium, SAS. or its affiliates. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <Python.h>

#include "handler_response.h"

#ifdef __cplusplus
extern "C" {
#endif

static PyObject *runtime_log_fn,
                *runtime_rtm_fn;

void runtime_log(char *msg)
{
    if (runtime_log_fn == NULL) {
        free(msg);
        return;
    }
    PyObject *tmp = PyObject_CallFunction(runtime_log_fn, "s", msg);
    if (tmp != NULL) {
        Py_DECREF(tmp);
    }
    free(msg);
}

long long runtime_rtm()
{
    long long res = 0;

    PyObject *tmp = PyObject_CallFunction(runtime_rtm_fn, NULL);
    if (tmp != NULL) {
        res = PyLong_AsLongLong(tmp);
        Py_DECREF(tmp);
    }

    return res;
}

static PyObject* runtime_open(PyObject *self, PyObject *args)
{
    const char *cnme, *cenv;
    char *err;

    if (!PyArg_ParseTuple(args, "ss", &cnme, &cenv)) {
        return NULL;
    }

    err = open_dynamic_library(cnme, cenv);
    //err = NULL;
    if (err) {
        PyErr_SetString(PyExc_ImportError, err);
        free(err);
        return NULL;
    }
    
    Py_RETURN_NONE;
}

static PyObject* runtime_lookup(PyObject *self, PyObject *arg)
{
    char *err;

    err = symbol_lookup(PyString_AS_STRING(arg));
    if (err) {
        PyErr_SetString(PyExc_AttributeError, err);
        free(err);
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* runtime_handle(PyObject *self, PyObject *args)
{
    const char *cevt, *cctx, *cenv;

    if (!PyArg_ParseTuple(args, "sssOO",
                          &cevt, &cctx, &cenv,
                          &runtime_log_fn, &runtime_rtm_fn)) {
        return NULL;
    }

    struct handler_return gres = handle_request(cevt, cctx, cenv);

    if (gres.error.message != NULL) {
        PyObject* err = PyErr_NewException(gres.error.type, NULL, NULL);
        PyErr_SetString(err, gres.error.message);
        free(gres.error.message);
        return NULL;
    }

    if (gres.payload != NULL) {
        PyObject* res = PyString_FromString(gres.payload);
        free(gres.payload);
        return res;
    }

    Py_RETURN_NONE;
}

static PyMethodDef runtime_methods[] = {
    {"open",   runtime_open,   METH_VARARGS},
    {"lookup", runtime_lookup, METH_O},
    {"handle", runtime_handle, METH_VARARGS},
    {NULL, NULL}
};

PyMODINIT_FUNC initruntime(void)
{
    Py_InitModule("runtime", runtime_methods);
}

#ifdef __cplusplus
}
#endif