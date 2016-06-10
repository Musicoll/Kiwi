#!/bin/bash -e

echo "Publish documentation..."
# Get doxygen
sudo apt-get install --yes doxygen graphviz

# Generate key
openssl aes-256-cbc -K $encrypted_5a43ff9033eb_key -iv $encrypted_5a43ff9033eb_iv -in travisci_rsa.enc -out travisci_rsa -d
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
echo "Ok."
