#!/bin/bash

if [[ $TRAVIS_BRANCH == 'dev-dsp' ]]; then
  $TRAVIS_SUBMODULES = false
elif [[ $TRAVIS_BRANCH == 'dev-core' ]]; then
  $TRAVIS_SUBMODULES = false
fi
