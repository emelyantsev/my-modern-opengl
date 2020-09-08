
// dynamic linking

nvcc -c kernel.cu -o kernel.o
nvcc -I"C:/SDK/opengl/glew-2.2.0/include" -I"C:/SDK/freeglut-3.2.1/include" -c main.cpp -o main.o -DNDEBUG
nvcc main.o kernel.o -o main.exe -L"C:/SDK/opengl/glew-2.2.0/lib/Release/x64" -lglew32 -L"C:/SDK/freeglut-3.2.1/build/lib/Release" -lfreeglut


// static linking

nvcc -c kernel.cu -o kernel.o
cl.exe main.cpp kernel.o C:\SDK\freeglut-3.2.1\build\lib\Release\freeglut_static.lib C:\SDK\opengl\glew-2.2.0\lib\Release\x64\glew32s.lib  ^
"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0\lib\x64\cudart_static.lib" ^
/std:c++17 /EHsc /IC:\SDK\freeglut-3.2.1\include /IC:\SDK\opengl\glew-2.2.0\include ^
/I"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.0/include"  ^
 /Femain.exe /DNDEBUG /DFREEGLUT_STATIC /MD /DGLEW_STATIC /link /NODEFAULTLIB:libcmt.lib /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup"





