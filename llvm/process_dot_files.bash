#!/usr/bin/env bash

# LLVM llc stores DAG graphs in /tmp
DIRECTORY=/tmp

# prepare local tmp
rmdir ./tmp
mkdir ./tmp

# copy graphviz dot files to local tmp, convert to png
for i in $DIRECTORY/*.dot; do
    # Process $i
    echo $i
    cp $i .$i
    rm $i
    dot -Tpng .$i > .$i.png
done
