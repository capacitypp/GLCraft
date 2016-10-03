@echo off
echo #ifndef ___Header_BuiltInImage > BuiltInImage.h
echo #define ___Header_BuiltInImage >> BuiltInImage.h
echo; >> BuiltInImage.h
..\..\ImageConverter blockCVIP
type output.c > BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockDirt1
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockDirt2
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockDirt3
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockLeaf
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic1
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic2
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic3
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic4
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic5
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic6
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic7
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic8
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic9
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockMosaic10
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockWood1
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter blockWood2
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
echo; >> BuiltInImage.h
echo #endif >> BuiltInImage.h
pause