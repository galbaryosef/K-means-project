from setuptools import Extension, setup
from glob import glob

c_files = glob('src/*.c')
c_files.extend(glob('python/*.c'))
c_files.append('spkmeansmodule.c')

module = Extension(
    "mykmeanssp",
    sources=c_files,
    include_dirs=['inc'])

setup(
    name='mykmeanssp',
    version='1.0',
    description='Python wrapper for custom C extension',
    ext_modules=[module])
