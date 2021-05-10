#!/bin/bash

# download site as-is
# if links conversion needed:  --convert-links

wget --recursive \
     --no-clobber \
     --page-requisites \
     --html-extension \
     --domains=bdpx.github.io \
     bdpx.github.io/postrisc/

