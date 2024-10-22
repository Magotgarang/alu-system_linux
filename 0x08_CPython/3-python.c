#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <string.h>

void print_python_list(PyObject *p) {
    if (!PyList_Check(p)) {
        printf("[.] list object info\n");
        printf("  [ERROR] Invalid List Object\n");
        return;
    }

    Py_ssize_t size = PyList_Size(p);
    Py_ssize_t allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(p, i);
        const char *type = Py_TYPE(item)->tp_name;
        printf("Element %zd: %s\n", i, type);
        
        // Additional details for specific types
        if (strcmp(type, "bytes") == 0) {
            print_python_bytes(item);
        } else if (strcmp(type, "float") == 0) {
            print_python_float(item);
        }
    }
}

void print_python_bytes(PyObject *p) {
    if (!PyBytes_Check(p)) {
        printf("[.] bytes object info\n");
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    Py_ssize_t size = PyBytes_Size(p);
    const char *bytes_string = PyBytes_AsString(p);

    printf("[.] bytes object info\n");
    printf("  size: %zd\n", size);
    printf("  trying string: %s\n", bytes_string);

    // Print first 10 bytes
    printf("  first %d bytes: ", size < 10 ? (int)size : 10);
    for (int i = 0; i < (size < 10 ? size : 10); i++) {
        printf("%02x ", (unsigned char)bytes_string[i]);
    }
    printf("\n");
}

void print_python_float(PyObject *p) {
    if (!PyFloat_Check(p)) {
        printf("[.] float object info\n");
        printf("  [ERROR] Invalid Float Object\n");
        return;
    }

    double value = PyFloat_AsDouble(p);
    printf("[.] float object info\n");
    printf("  value: %f\n", value);
}
