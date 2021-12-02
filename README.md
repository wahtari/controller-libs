# controller-libs
Contains C/C++ language bindings to the internal Wahtari controller-lib.  

## Quick Start
**Attention!** Only run the samples in this repository on compatible hardware.  
The samples are interacting with GPIO Pins. If the underlying hardware (e.g. an average Laptop/Desktop) does not support this, serious damage can occur to the system.  
***Use at your own risk!***

### C
Find the sample C application [here](https://github.com/wahtari/controller-libs/blob/master/c/sample/main.c).  
You can build the sample with this command fromm the root of this repo:  
```bash
gcc \
    -Wall \
    -Wextra \
    -I c \
    -L c \
    -o controller-libs-sample-c \
    c/sample/main.c \
    -lnlab-ctrl
```
Run the sample: `chmod +x controller-libs-sample-c && ./controller-libs-sample-c`

### C++
Find the sample C++ application [here](https://github.com/wahtari/controller-libs/blob/master/cpp/sample/main.cpp).  
You can build the sample with this command fromm the root of this repo:  
```bash
g++ \
    -Wall \
    -Wextra \
    -I cpp \
    -I c \
    -L cpp \
    -L c \
    -o controller-libs-sample-cpp \
    cpp/sample/main.cpp \
    -lnlab-ctrl-cpp \
    -lnlab-ctrl
```
Run the sample: `chmod +x controller-libs-sample-cpp && ./controller-libs-sample-cpp`

## Documentation
- [C API](https://docs.wahtari.io/controller-libs/libnlab-ctrl_8h.html)
- [C++ API](https://docs.wahtari.io/controller-libs/libnlab-ctrl_8hpp.html)

## License
The license for this product can be found in the `LICENSE` file.  
The licenses for all third-party dependencies are listed in the `LICENSE-deps` file.
