# CG-Project----Volume-Renderer

Instruction to run the code. 

1. Open Project in QtCreator.
2. Run the Project.

Intruction to load different volume

1. Place the volume file in the project folder (the one with the .pro file)
2. In the openglwidget.cpp file's initializeGL() method, in the call to loadVolume
    replace the name of the file with the parameter. In the same method replace 
    texW, texH, texD with the respective width, height and depth of the volume to load

Note : We used Qt 5.13 in Ubuntu 18.04