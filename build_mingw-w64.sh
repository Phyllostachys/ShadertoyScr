g++ -Wall -o bin/ShadertoyScr.exe -I include -I src -L lib/mingw-w64 src/glad.c src/shader.cpp src/main.cpp -lglfw3dll

if [ -f bin/glfw3.dll ]; then
    echo "bin/glfw3.dll already exists"
else
    cp lib/mingw-w64/glfw3.dll bin
fi

if [ -d bin/shaders ]; then
    echo "bin/shaders already exists"
else
    cp shaders/ bin/shaders
fi
