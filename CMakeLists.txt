cmake_minimum_required (VERSION 2.6)

FIND_PACKAGE(Torch REQUIRED)
#name of .so lib, which will be in /lib/lua/5.1

set (libname wta)
set (src wta.c)
set (luasrc init.lua)

ADD_TORCH_PACKAGE("${libname}" "${src}" "${luasrc}" "wta")




