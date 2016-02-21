#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
fi

TARGET="test_core test_dsp"
PERFORM_TESTS="./test_core ./test_dsp"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  TARGET="test_dsp"
  PERFORM_TESTS="./test_dsp"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  TARGET="test_core"
  PERFORM_TESTS="./test_core"
fi

echo "---------------------"
echo "       SCRIPT        "
echo "---------------------"
echo "OS=" $TRAVIS_OS_NAME
echo "BRANCH=" $TRAVIS_BRANCH
echo "COMPILER=" $COMPILER
echo "COVERALL_SUPPORT=" $COVERALL_SUPPORT
echo "TARGET="$TARGET
echo "TEST="$PERFORM_TESTS
echo "-------------------"

export CXX=$COMPILER
make clean
cmake -DCOVERALL=$COVERALL_SUPPORT ./
cmake --build ./ --target $TARGET
$PERFORM_TESTS

if [ $COVERALL_SUPPORT == "ON" ]; then
  coveralls -E '/build/CMakeFiles/*.cpp' -E '/build/CMakeFiles/*.cxx' -e ThirdParty -e Client -e test --gcov-options '\-lp' --gcov 'gcov-4.9'
fi
