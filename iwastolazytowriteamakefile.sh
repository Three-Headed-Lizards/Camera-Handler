#!/usr/bin/env sh

######################################################################
# @author      : theo (theo.j.lincke@gmail.com)
# @file        : iwastolazytowriteamakefile
# @created     : Sunday Oct 27, 2019 17:55:22 MDT
#
# @description : compiler
######################################################################

g++ ./src/april.cc ./src/posthandler.c -I ./include/ -isystem /usr/include/opencv4 -O3 -std=gnu++11 -o run -lapriltag `pkg-config --libs opencv4` -lcurl



