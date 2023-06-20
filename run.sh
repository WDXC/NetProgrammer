
#!/bin/bash
if [ ! -d ./build ];then
    cmake -B build .
fi
cd build
make clean
make -j 1
cp ./bin/*_main ../
    
