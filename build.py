#!/usr/bin/python3
import sys
import os

def main():
    # cmake Release
    os.system('cmake -B build -S . -DCMAKE_BUILD_TYPE=Release')
    # cmake debug
    # os.system('cmake -B build -S .')
    os.system('cmake --build build --config Release --parallel $(nproc) ')
    # os.system('./build/RayTracing ' + ' '.join(sys.argv[1:]))

if __name__ == "__main__":
    main()