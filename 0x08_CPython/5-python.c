#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <limits.h> // For ULONG_MAX

void print_python_int(PyObject *p) {
    if (!PyLong_Check(p)) {
        printf("[.] int object info\n");
        printf("  [ERROR] Invalid Int Object\n");
        return;
    }

    unsigned long value = PyLong_AsUnsignedLong(p);
    if (value == ULONG_MAX && PyErr_Occurred()) {
        // Check for overflow
        printf("C unsigned long int overflow\n");
        PyErr_Clear(); // Clear the error
    } else {
        printf("%lu\n", value);
    }
}

