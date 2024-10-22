#include <Python.h>
#include <object.h>
#include <listobject.h>

/**
 * print_python_list - Prints basic information about Python lists
 * @p: Pointer to the PyObject (Python list object)
 */
void print_python_list(PyObject *p)
{
    Py_ssize_t size, i;
    Py_ssize_t allocated;
    const char *type;

    size = PyList_Size(p);
    allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    for (i = 0; i < size; i++)
    {
        type = (Py_TYPE(PyList_GetItem(p, i)))->tp_name;
        printf("Element %zd: %s\n", i, type);
    }
}

