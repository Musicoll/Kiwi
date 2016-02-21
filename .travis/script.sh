#!/bin/bash

COVERALL_SUPPORT="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORT="ON"
  COVERALL_EXCEPTCPP="\'/CMakeFiles/*.cpp\'"
  COVERALL_EXCEPTCXX="\'/CMakeFiles/*.cxx\'"
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

echo ""
echo "---------------------"
echo "TARGET="$TARGET
echo "TEST="$PERFORM_TESTS
echo "---------------------"
echo ""

export CXX=$COMPILER
make clean
cmake -DCOVERALL=$COVERALL_SUPPORT ./
cmake --build ./ --target $TARGET
$PERFORM_TESTS
