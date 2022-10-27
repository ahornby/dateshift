#!/usr/bin/env bash

case $(uname) in 
    Darwin*)
        glibtoolize -ci
        ;;
    *)
        libtoolize -ci
        ;;        
esac

aclocal && automake --copy --add-missing && autoconf

