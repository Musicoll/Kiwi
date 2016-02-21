#!/bin/bash

if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
  export CXX=$COMPILER
  cmake ./
  cmake --build ./  --target test_dsp
elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
  export CXX=$COMPILER
  cmake ./
  cmake --build ./  --target test_core
else
  export CXX=$COMPILER
  cmake ./
  cmake --build ./
fi
