@echo off
echo #ifndef ___Header_BuiltInImage > BuiltInImage.h
echo #define ___Header_BuiltInImage >> BuiltInImage.h
echo; >> BuiltInImage.h
..\..\ImageConverter itemSlot1
type output.c > BuiltInImage.cpp
type output.h >> BuiltInImage.h
..\..\ImageConverter itemSlot2
type output.c >> BuiltInImage.cpp
type output.h >> BuiltInImage.h
echo; >> BuiltInImage.h
echo #endif >> BuiltInImage.h
pause