Requirements
============

   This installation requires the pre-installation of the cmake tool,
   a C++11 ready compiler such as g++ version 4.7, and the library: sdsl.

   For Linux, the library sdsl will be installed while makin the tool itself as given below.


Basic Instructions
==================
To install sdsl library (if not already installed)
```sh
cd ./external/sdsl-lite
./install.sh `pwd`
```

   To compile the tool, use the following shell command:
```sh
make -f Makefile.gcc 
```
   

	After compilation the binary `eldes` will be created in `bin` folder of the working 
directory; the tool can be used thereafter. 
You can try it with the given sample files (`sample` folder) via
```sh
./bin/eldes -a DNA -t ./sample/sampleText.txt -p ./sample/samplePattern.txt -o ./sample/sampleOutput.txt
```
Test
====

The [test](./test) directory contains test code. [googletest][GTEST]
framework and [make][MAKE] have been used to run the tests. 

To install googletest locally, use the following command:
```sh
cd ./external/googletest
cmake .
make
mv libg* ../gtest/lib/
cp -R include/gtest ../gtest/include
```

To simply run all unit tests after compiling the tool, use the following command

```sh
cd ./test
make -f Makefile.gcc
./bin/mytests
```


