#!/bin/bash

COVERALL_SUPPORT = -DCOVERALL=OFF
if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]]; then
  $COVERALL_SUPPORT = -DCOVERALL=ON
fi

TEST_SUPPORT
if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
  $TEST_SUPPORT = --target  --target test_dsp
elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
  $TEST_SUPPORT = --target  --target test_core
fi

export CXX=$COMPILER
make clean
cmake $COVERALL_SUPPORT ./
cmake --build ./  $TEST_SUPPORT
