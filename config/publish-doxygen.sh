#!/bin/bash

# Get a clean version of the HTML documentation repo.
ls
rm -rf html
mkdir -p html
git clone -b gh-pages git@github.com:Musicoll/Kiwi.git --single-branch html

# rm all the files through git to prevent stale files.
cd html
echo "list files..."
ls
echo "end"
git rm -rf .
echo "list files..."
ls
echo "end"
cd -

# Generate the HTML documentation.
doxygen Doxyfile

# Create and commit the documentation repo.
cd html
echo "list files..."
ls
echo "end"
git add .
git commit -m "Automated documentation build."
git push origin gh-pages
cd -
