#!/bin/sh

set -xe

CC=clang
CXXFLAGS="-Wall -Wextra"
CPPFLAGS=-std=c++17
LIBS=-lm
SRC=main.cpp

$CC $CXXFLAGS $CPPFLAGS $SRC -o MonteCarlo $LIBS
