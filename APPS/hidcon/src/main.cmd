mkdir ..\x64-Release\src
C:\mingw\bin\windres.exe -J rc -O coff -F pe-x86-64 -i resource.rc -o ../x64-Release/src/resource.res
mkdir ..\x32-Release\src
C:\mingw\bin\windres.exe -J rc -O coff -F pe-i386 -i resource.rc -o ../x32-Release/src/resource.res
