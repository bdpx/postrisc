#!/usr/bin/env bash

cd llvm-project
cd llvm
cd lib
cd Target
cd Postrisc

# root
for i in Sparc*;
do
    echo rename ${i}
    mv ${i} ${i/#Sparc/Postrisc};
done
for i in *;
do
    echo replace ${i}
    sed -i "s/SPARC/POSTRISC/g" ${i}
    sed -i  "s/Sparc/Postrisc/g" ${i}
    sed -i "s/sparc/postrisc/g" ${i}
done

cd AsmParser
for i in Sparc*;
do
    echo rename ${i}
    mv ${i} ${i/#Sparc/Postrisc};
done
for i in *;
do
    echo replace ${i}
    sed -i "s/SPARC/POSTRISC/g" ${i}
    sed -i  "s/Sparc/Postrisc/g" ${i}
    sed -i "s/sparc/postrisc/g" ${i}
done
cd ..

cd Disassembler
for i in Sparc*;
do
    echo rename ${i}
    mv ${i} ${i/#Sparc/Postrisc};
done
for i in *;
do
    echo replace ${i}
    sed -i "s/SPARC/POSTRISC/g" ${i}
    sed -i  "s/Sparc/Postrisc/g" ${i}
    sed -i "s/sparc/postrisc/g" ${i}
done
cd ..

cd MCTargetDesc
for i in Sparc*;
do
    echo rename ${i}
    mv ${i} ${i/#Sparc/Postrisc};
done
for i in *;
do
    echo replace ${i}
    sed -i "s/SPARC/POSTRISC/g" ${i}
    sed -i  "s/Sparc/Postrisc/g" ${i}
    sed -i "s/sparc/postrisc/g" ${i}
done
cd ..

cd TargetInfo
for i in Sparc*;
do
    echo rename ${i}
    mv ${i} ${i/#Sparc/Postrisc};
done
for i in *;
do
    echo replace ${i}
    sed -i "s/SPARC/POSTRISC/g" ${i}
    sed -i  "s/Sparc/Postrisc/g" ${i}
    sed -i "s/sparc/postrisc/g" ${i}
done
cd ..
