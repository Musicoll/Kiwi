#!/bin/bash

if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
  export CXX=$COMPILER
  make clean
  if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]]; then
  cmake ./
  else
  cmake -DCOVERALL=ON ./
  fi
  cmake --build ./  --target test_dsp
elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
  export CXX=$COMPILER
  make clean
  if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]]; then
  cmake ./
  else
  cmake -DCOVERALL=ON ./
  fi
  cmake --build ./  --target test_core
else
  export CXX=$COMPILER
  make clean
  if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]]; then
  cmake ./
  else
  cmake -DCOVERALL=ON ./
  fi
  cmake --build ./
fi
