#!/bin/bash

if [[ "$TRAVIS_BRANCH" == "master" ]];
then
# Get a clean version of the HTML documentation repo.
cd config
rm -rf html
mkdir -p html
git clone -b gh-pages git@github.com:Musicoll/Kiwi.git --single-branch html

# rm all the files through git to prevent stale files.
cd html
git rm -rf .
cd -

# Generate the HTML documentation.
doxygen Doxyfile

# Create and commit the documentation repo.
cd html
git add .
git commit -m "Automated documentation build."
git push origin gh-pages
cd -
fi
