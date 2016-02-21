#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == 'gcc' ]]; then
coveralls -E '/build/CMakeFiles/*.cpp' -E '/build/CMakeFiles/*.cxx' -e ThirdParty -e Client -e test --gcov-options '\-lp' --gcov 'gcov-4.9'
else
  if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
    ./test_dsp
  elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
    ./test_core
  else
    ./test_core
    ./test_dsp/
  fi
fi
