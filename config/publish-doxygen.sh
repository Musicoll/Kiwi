#!/bin/bash -e

# Move key
chmod 0600 travisci_rsa
cp travisci_rsa ~/.ssh/id_rsa

# Get a clean version of the HTML documentation repo.
rm -rf html
mkdir -p html
git clone -b gh-pages git@github.com:Musicoll/Kiwi.git --single-branch html

# rm all the files through git to prevent stale files.
cd html
git rm -rf .
cd -

# Generate the HTML documentation.
make doxygen

# Create and commit the documentation repo.
cd html
git add .
git commit -m "Automated documentation build."
git push origin gh-pages
cd -
