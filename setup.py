from distutils.core import setup, Extension
import os


PACKAGE = 'termkey'
SOURCES = [
    'termkey/mod_termkey.c',
]

README = open('README.rst').read().strip() if os.path.isfile('README.rst') else ''

termkey_module = Extension('termkey._termkey', SOURCES,
                           libraries=['termkey'],
                           language='c')

setup(name='pytermkey',
      version='0.1',
      author='Sergey Shepelev',
      author_email='temotor@gmail.com',
      url='http://github.com/temoto/pytermkey',
      packages=[PACKAGE],
      description="pytermkey is python binding to libtermkey library by Paul Evans.",
      long_description=README,
      ext_modules=[termkey_module],
      license='MIT License',
     )
