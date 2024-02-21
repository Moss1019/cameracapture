[ -x ./build ] || mkdir build

cp ./vertex.glsl ./build
cp ./fragment.glsl ./build
cd build
cmake ..
make && ./cameradisplay $1
cd ..