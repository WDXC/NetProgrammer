#!/bin/bash

dir="./build"
file="./run.sh"
cache="./.cache"

echo -e "1. Generate compile script\n"
echo -e "2. Commit your local commit\n"
echo -e "3. Quit(q)\n"

generate_compile_script()
{
    if [ -f "run.sh" ]; then
        echo -e "Compile shell script has been generated at: $PWD/run.sh\n"
        exit
    fi
    echo "
#!/bin/bash
if [ ! -d "$dir" ];then
    cmake -B build .
fi
cd build
make clean
make -j $(nproc || sysctl -n hw.ncpu || echo 2)
    " > run.sh
    chmod +x run.sh
    echo -e "Compile shell script generated at: $PWD/run.sh\n"
}

commit_local_code()
{
    read -p "Please input commit context: " context 

    if [ -f "$file" ];then
        rm -rf $file
    fi
    if [ -d "$dir" ];then
        rm -rf $dir
    fi
    if [ -d "$cache" ];then
        rm -rf $cache
    fi

    git add .
    git commit -m "${context}"
    git push origin main
}


while true; do
    read -p "Enter your choice: " choice
    case $choice in
        "1")
            generate_compile_script
            break
            ;;
        "2")
            commit_local_code
            break
            ;;
        "q")
            exit
            break
            ;;
        *)
            echo -e "Please input a correct choice!!!\n"
            ;;
    esac
done

