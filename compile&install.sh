#!/bin/bash

g++ -Wall -Wextra -Wpedantic -Werror -lcurl -o GPTerminal GPTerminal.cpp

if [ $? -eq 0 ]; then
    sudo cp GPTerminal /usr/bin/
    hash -r
    echo "GPTerminal has been successfully installed. To start it, use the command: GPTerminal"
else
    echo "Compilation failed. Please check your source code for errors. Or contact me on Discord: shookx_"
fi
