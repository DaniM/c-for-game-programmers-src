echo off

mkdir build
pushd src\plugins
call builddlls.bat
popd

pushd build

copy ..\src\plugins\build\*.dll .
cl -FAsc -Zi /c /I..\inc /EHsc ..\src\pluginmanager.cpp
cl -FAsc -Zi /I..\inc /EHsc ..\tests\test_plugins.cpp pluginmanager.obj
popd
