#!/usr/bin/env python

##### IMPORT #################################################################

import sys

from subprocess import call

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

##### GEN DOC #################################################################

TITLE = "Kiwi"
OUTPUT_FILENAME = "index"
INPUT_PATH = "./Resources/doctogen"
OUTPUT_PATH = "./docs"

call("gwtc -t " + TITLE + " -n " + OUTPUT_FILENAME + " -o " + OUTPUT_PATH + " " + INPUT_PATH, shell=True)
