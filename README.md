# C Terminal 3d Renderer

Naively renders colored points to the terminal and animates them. Currently configured to display a pointcloud of a 'donut'.

This project requires GCC (clang may work) to be built and is designed to be run on linux. There is a provided makefile.

## Note

In this project I used the convention of naming structs `struct_t` which I have learned is bad practice. Additionally, structs are typedef'd which is also bad practice.