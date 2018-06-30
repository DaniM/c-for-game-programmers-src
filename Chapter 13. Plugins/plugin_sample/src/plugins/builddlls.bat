echo off

mkdir build
pushd build

cl -FAsc -Zi /LDd /I..\..\..\inc /DDLL /EHsc ..\fooplugin.cpp

popd
