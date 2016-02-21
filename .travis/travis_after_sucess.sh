#!/bin/bash

if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]
  ./test_dsp
else if [[ $TRAVIS_BRANCH == 'dev-core' ]]
  ./test_core
else
  ./test_core
  ./test_dsp/
fi
