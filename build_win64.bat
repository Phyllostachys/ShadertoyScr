cl /Wall /wd4054 /wd4055 /wd4100 /wd4255 /wd4267 /wd4365 /wd4365 /wd4530 /wd4577 /wd4710 /wd4820 /wd4996 /Fe:bin\ShadertoyScr.exe /Iinclude /Isrc src\glad.c src\shader.cpp src\main.cpp lib\win64\glfw3dll.lib

copy lib\win64\glfw3.dll bin\
xcopy shaders\* bin\shaders\ /s /i
