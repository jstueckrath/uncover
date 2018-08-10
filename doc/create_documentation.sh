#!/bin/sh
rm -rf html
rm -rf latex
doxygen doc_conf.txt
exit $?
