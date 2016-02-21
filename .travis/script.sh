#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]; then
  $COVERALL_SUPPORT="ON"
fi



TEST_RUN="test_core test_dsp"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  $TEST_RUN="test_dsp"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  $TEST_RUN="test_core"
fi

echo "COVERALL_SUPPORT=" $COVERALL_SUPPORT
echo "TEST_RUN="$TEST_RUN

export CXX=$COMPILER
make clean
cmake -DCOVERALL=$COVERALL_SUPPORT ./
cmake --build ./ --target $TEST_RUN
