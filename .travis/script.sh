#!/bin/bash

COVERALL_SUPPORT = 'OFF'
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]; then
  $COVERALL_SUPPORT = '-DCOVERALL=ON'
fi

TEST_SUPPORT = 'test_core test_dsp'
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  $TEST_SUPPORT = 'test_dsp'
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  $TEST_SUPPORT = 'test_core'
fi

export CXX=$COMPILER
make clean
cmake -DCOVERALL=$COVERALL_SUPPORT ./
cmake --build ./ --target $TEST_SUPPORT
