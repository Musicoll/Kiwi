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
elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
  TARGET="test_graphics"
  PERFORM_TESTS="./test_graphics"
fi

echo ""
echo "---------------------"
echo "TARGET="$TARGET
echo "TEST="$PERFORM_TESTS
echo "---------------------"
echo ""

mkdir build
cd build
export CXX=$COMPILER
cmake -DCOVERALL=$COVERALL_SUPPORT ..
cmake --build ./ --target $TARGET
make clean
$PERFORM_TESTS
