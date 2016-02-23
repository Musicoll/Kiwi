#!/bin/bash

COVERALL_SUPPORTS="OFF"
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'g++-4.9' ]; then
  COVERALL_SUPPORTS="ON"
  COVERALL_EXCEPTCPP="\'/CMakeFiles/*.cpp\'"
  COVERALL_EXCEPTCXX="\'/CMakeFiles/*.cxx\'"
fi

TARGETS="test_core test_dsp"
PERFORM_TESTS="./test_core ./test_dsp"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  TARGETS="test_dsp"
  PERFORM_TESTS="./test_dsp"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  TARGETS="test_core"
  PERFORM_TESTS="./test_core"
elif [ $TRAVIS_BRANCH == 'dev-graphics' ]; then
  TARGETS="test_graphics"
  PERFORM_TESTS="./test_graphics"
fi

echo ""
echo "---------------------"
echo "TARGETS="$TARGETS
echo "---------------------"
echo ""

mkdir build
cd build
export CXX=$COMPILER
cmake -DCOVERALL=$COVERALL_SUPPORTS ..
make $TARGETS
for TEST in $PERFORM_TESTS; do
$TEST
done
