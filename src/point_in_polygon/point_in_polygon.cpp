// CPython extension for checking wether or not a point (x, y)
//  located within a polygon.
// source: https://wrfranklin.org/Research/Short_Notes/pnpoly.html
// with slight modification.

#include <python3.10/Python.h>
#include <vector>


#ifdef __cplusplus
extern "C" {
#endif

static PyObject* point_in_polygon(PyObject* self, PyObject* args)
{
    int n_vert;
    PyObject *polygon_x;
    PyObject *polygon_y;
    double test_x, test_y;
   /* Parse the input tuple */
    if (!PyArg_ParseTuple(args, "iOOdd", &n_vert, &polygon_x, &polygon_y, &test_x, &test_y))
        return NULL;
    PyObject *iterator_polygon_x = PyObject_GetIter(polygon_x);
    if (!iterator_polygon_x)
        return NULL;
    PyObject *iterator_polygon_y = PyObject_GetIter(polygon_y);
    if (!iterator_polygon_y)
        return NULL;

    std::vector<double> poly_array_x, poly_array_y;

    for (int N=0; N<n_vert; ++N) {
        poly_array_x.push_back(PyFloat_AsDouble(PyIter_Next(iterator_polygon_x)));
        poly_array_y.push_back(PyFloat_AsDouble(PyIter_Next(iterator_polygon_y)));
    }

    if (poly_array_x[0] != poly_array_x[n_vert-1]) {
        PyErr_SetString(PyExc_ValueError, "Polygon is not closed.");
        return NULL;
    }
    if (poly_array_y[0] != poly_array_y[n_vert-1]) {
        PyErr_SetString(PyExc_ValueError, "Polygon is not closed.");
        return NULL;
    }

    int i, j, c = 0;
    for (i = 0, j = n_vert-1; i < n_vert; j = i++) {
        if ( ((poly_array_y[i]>test_y) != (poly_array_y[j]>test_y)) &&
    	     (test_x < (poly_array_x[j]-poly_array_x[i]) * (test_y-poly_array_y[i]) / (poly_array_y[j]-poly_array_y[i]) + poly_array_x[i]) )
            c = !c;
    }

    return c ? Py_True: Py_False;
}


static PyMethodDef pip_methods[] = {
    { "point_in_polygon", point_in_polygon, METH_VARARGS,
    "Checks if a point is located within a polygon.\n"
    "The polygon must be closed, i.e. the last point must be identical to the\n"
    "first point. polygon_x[0] == polygon_x[-1] and polygon_y[0] == polygon_y[-1]."
    "\n"
    "Parameters\n"
    "----------\n"
    "length : int\n"
    "    Number of vertices in the polygon.\n"
    "polygon_x : list\n"
    "    List of x coordinates of the polygon vertices. Must have length 'length'.\n"
    "polygon_y : list\n"
    "    As polygon_x, but for the y coordinates.\n"
    "test_x : float\n"
    "    X coordinate of the test point.\n"
    "test_y : float\n"
    "    Y coordinate of the test point.\n"
    "\nReturns\n"
    "-------\n"
    "is_contained : bool\n"
    "    True if the polygon contains the test point.\n"
    "\n"
    "Edge Cases\n"
    "----------\n"
    "    Points on a right or upwards facing edges are considered\n"
    "    outside the polygon, points on left or downards facing\n"
    "    edges are considered inside.\n"
    "    Vertices which are on the top-left \n"
    "\n"
    "Raises\n"
    "------\n"
    "ValueError\n"
    "    If the polygon is not closed.\n"
    "\n" },
    { NULL, NULL, 0, NULL }
};
// Edge Cases
//     ----------
//     Left edge: INSIDE
//     Top edge: OUTSIDE
//     Right edge: OUTSIDE
//     Bottom edge: INSIDE
// Our Module Definition struct
static struct PyModuleDef pip_module = {
    PyModuleDef_HEAD_INIT,
    "point_in_polygon",
    "Test Module",
    -1,
    pip_methods
};


PyMODINIT_FUNC PyInit_point_in_polygon(void)
{
    return PyModule_Create(&pip_module);
}

#ifdef __cplusplus
}  // closes extern "C"
#endif
