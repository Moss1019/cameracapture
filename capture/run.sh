[ -x ./build ] || mkdir build

cd build
cmake ..
make && ./cameracapture $1
cd ..