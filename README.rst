What
====

Python C extension bindings for libtermkey, one excellent terminal key handling library.

This binding is similar to ctypes_termkey, but in form of a C extension.
Meaning, you will need a C compiler and libtermkey headers to build the binding, but it will work
faster than ctypes version.


Demo
====

See demo.py for a simple demo (to exit press C-c, that is Control+C).


Usage
=====

To use bindings, you must first install `libtermkey <http://www.leonerd.org.uk/code/libtermkey/>`_.

Then, you can import `TermKey` from termkey::

    from termkey import TermKey

if all goes ok, you'll see no errors here.

You can now instantiate `TermKey` class to start using libtermkey::

    tk = TermKey(0, 0)

Usage of class is meaningful here, because it holds a state (an opaque handle to libtermkey) and you can create
more than one `TermKey` to different file descriptors, etc.

You can pass two parameters to constructor: file descriptor and flags.

*TODO document parameters*

Now you can start collecting keys, i.e. in wait mode::

    result, key = tk.waitkey()

to check result compare it to `termkey` attributes `RES_NONE`, `RES_KEY`, `RES_EOF` and `RES_AGAIN`::

    from termkey import RES_KEY
    if result == RES_KEY:
        print "got key"

key is a `termkey.Key` instance.

To get string key name from key, you can call `str(key)` on it::

    print str(key)

which could print, i.e., q.

Look at termkey documentation to learn how to use it.


Credits
=======

Bindings by Sergey Shepelev <temotor@gmail.com>

libtermkey by Paul LeoNerd Evans <leonerd@leonerd.org.uk>


License
=======

pytermkey is under MIT license. Text is included in file LICENSE.
