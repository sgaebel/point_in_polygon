from distutils.core import setup, Extension

polygon_module = Extension('point_in_polygon',
                           sources = ['point_in_polygon.c'])

setup (name = 'point_in_polygon',
       version = '1.0',
       description = 'Package for polygon functions.',
       ext_modules = [polygon_module])
