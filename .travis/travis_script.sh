#!/bin/bash

if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]
  export CXX=$COMPILER
  cmake ./
  cmake --build ./  --target test_dsp
else if [[ $TRAVIS_BRANCH == 'dev-core' ]]
  export CXX=$COMPILER
  cmake ./
  cmake --build ./  --target test_core
else 
  export CXX=$COMPILER
  cmake ./
  cmake --build ./
fi
