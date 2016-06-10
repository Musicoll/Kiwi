#!/bin/bash -e

mv Client/Builds/MacOSX/build/Debug/Kiwi.app Kiwi.app
zip -r Kiwi.zip Kiwi.app
