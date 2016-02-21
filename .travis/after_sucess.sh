#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'linux' ] && [ $COMPILER == "g++4.9"]]; then
  if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
    ./test_dsp
  elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
    ./test_core
  else
    ./test_core
    ./test_dsp/
  fi
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
