@echo off

set AdditionalIncludePath=-I..\..\Imgui\ -I..\..\Imgui\*.cpp
set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -FAsc -Z7 -DDATA_SNIFFER_DEBUG -DCOMPILER_MSVC
set CommonLinkerFlags=-opt:ref user32.lib Gdi32.lib winmm.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

cl %CommonCompilerFlags% %AdditionalIncludePath% ..\src\win32_data_sniffer.cpp /link -subsystem:windows,"5.02" %CommonLinkerFlags%
popd