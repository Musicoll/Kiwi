#!/bin/bash -e

mv Build/Release/KiwiBuild/Bin/MacRelease/Kiwi.app Kiwi.app
zip -r Kiwi.zip Kiwi.app
