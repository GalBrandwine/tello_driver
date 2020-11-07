# tello_driver (cpp)
A **cpp** Tello_driver for the DJI tello drone, that supports the **unofficial** SDK.
Meant to be included as a library, to other projects. See examples.

Installation
---
```shell
git clone https://github.com/GalBrandwine/tello_driver.git
cd tello_driver
mkdir build && cd build
cmake ..  && make -j # Or, for debugging: cmake .. -DCMAKE_BUILD_TYPE=Debug && make -j
```

# Running the examples
```shell
cd build/examples
./simple_connection

