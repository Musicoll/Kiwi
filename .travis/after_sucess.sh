#!/bin/bash

TEST_PERFORM="./test_core ./test_dsp"
if [ $TRAVIS_BRANCH == 'dev-dsp' ]; then
  $TEST_PERFORM="./test_dsp"
elif [ $TRAVIS_BRANCH == 'dev-core' ]; then
  $TEST_PERFORM="./test_core"
fi

$TEST_PERFORM
if [ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]; then
  coveralls -E '/build/CMakeFiles/*.cpp' -E '/build/CMakeFiles/*.cxx' -e ThirdParty -e Client -e test --gcov-options '\-lp' --gcov 'gcov-4.9'  
fi
