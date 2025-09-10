#!/bin/sh

gcc simple.c  simple.pb.c nanopb/*.c -I nanopb/ -o test
