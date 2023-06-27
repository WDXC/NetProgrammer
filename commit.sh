#!/bin/bash

shopt -s nullglob

dir="./build"
file="./run.sh"
cache="./.cache"


echo -e "1. Generate compile script\n"
echo -e "2. Commit your local commit\n"
echo -e "3. clear local development envrioment\n"
echo -e "q. Quit(q)\n"

generate_compile_script()
{
    echo -e "\n"
    if [ -f "run.sh" ]; then
        echo -e "Compile shell script has been generated at: $PWD/run.sh\n"
        exit
    fi
    cat << EOF > run.sh
#!/bin/bash
if [ ! -d "$dir" ];then
    cmake -B build .
fi
cd build
make clean
make -j \$(nproc || sysctl -n hw.ncpu || echo 2)
echo

echo "executable file at: \${PWD}/bin"
EOF
    chmod +x run.sh
    echo -e "\nCompile shell script generated at: $PWD/run.sh\n"
}

commit_local_code()
{
    echo  -e "\n"
    read -p "Please input commit context: " context 

    git add .
    git commit -m "${context}"
    git push origin main
}

clear_development_envrionment()
{
    echo -e "\n"
    if [ -d ${dir} ];then
        rm -rf ${dir}
    fi
    if [ -d ${cache} ];then
        rm -rf ${cache}
    fi
    if [ -f ${file} ];then
        rm ${file}
    fi
    if bin_file=(*_main); [ ${#bin_file[@]} -gt 0 ]; then
        rm "${bin_file[@]}"
    fi
    echo -e "clean successfully\n"
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
        "3")
            clear_development_envrionment
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

