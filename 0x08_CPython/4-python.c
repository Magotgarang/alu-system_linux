#define PY_SSIZE_T_CLEAN
#include <Python.h>

void print_python_string(PyObject *p) {
    if (!PyUnicode_Check(p)) {
        printf("[.] string object info\n");
        printf("  [ERROR] Invalid String Object\n");
        return;
    }

    Py_ssize_t length = PyUnicode_GetLength(p);
    const char *value = PyUnicode_AsUTF8(p);
    const char *type = (PyUnicode_KIND(p) == PyUnicode_1BYTE_KIND) ? "compact ascii" :
                       (PyUnicode_KIND(p) == PyUnicode_2BYTE_KIND) ? "compact unicode object" : "compact unicode object";

    printf("[.] string object info\n");
    printf("  type: %s\n", type);
    printf("  length: %zd\n", length);
    printf("  value: %s\n", value);
}

