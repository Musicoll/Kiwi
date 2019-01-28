#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import argparse
import binascii

##### VERSION #################################################################

if sys.version_info[0] != 2 or sys.version_info[1] < 7:
    print("This script requires version python 2, 2.7 or higher")
    sys.exit(1)

#==============================================================================
# Name : global variables
#==============================================================================

tab_count = 0
DEFAULT_INPUT_PATH = "./Resources/BinaryRes"
DEFAULT_OUTPUT_PATH = "./Client/Source/KiwiApp_Resources"

#==============================================================================
# Name : parse_args
#==============================================================================

def parse_args ():

    arg_parser = argparse.ArgumentParser()

    arg_parser.add_argument('-i', '--input',
                            nargs='?', const=DEFAULT_INPUT_PATH, default=DEFAULT_INPUT_PATH, type=str,
                            help="input directory in which to search ressources")

    arg_parser.add_argument('-o', '--output',
                            nargs='?', const=DEFAULT_OUTPUT_PATH, default=DEFAULT_OUTPUT_PATH, type=str,
                            help="output directory for generated files")

    return arg_parser.parse_args(sys.argv[1:])

#==============================================================================
# Name : tabulate
#==============================================================================

def write_with_tab(file, message):
    global tab_count
    for tab in range(0, tab_count): file.write("    ")
    file.write(message)
    return

def inc_tab():
    global tab_count
    tab_count += 1
    return

def dec_tab():
    global tab_count
    tab_count -= 1
    return

#==============================================================================
# Name : Extract binary data
#==============================================================================

def extract_binary_data(output_h_file, output_cpp_file, input_file_path):

    file = open(input_file_path, "rb")

    filename = os.path.basename(input_file_path).replace(".", "_").replace("-", "_")

    # Read all bytes
    bytes = file.read()
    size = len(bytes)

    # Write header file info
    write_with_tab(output_h_file, "extern char const* " + filename + ";\n")
    write_with_tab(output_h_file, "const int " + filename + "_size = " + str(size) + ";\n\n")

    # Write cpp file info
    write_with_tab(output_cpp_file, "static const unsigned char " + filename + "_array[] = \n")
    write_with_tab(output_cpp_file, "{\n");

    line_counter = 0

    for byte_index in range(0, size - 1):
        byte_hexa = binascii.hexlify(bytes[byte_index])
        output_cpp_file.write("0x" + byte_hexa + ", ")
        line_counter = line_counter + 1
        if line_counter > 15:
            output_cpp_file.write("\n");
            line_counter = 0;

    output_cpp_file.write("0x" + binascii.hexlify(bytes[size - 1]) + "\n")
    write_with_tab(output_cpp_file, "};\n\n")
    write_with_tab(output_cpp_file, "const char* " + filename + " = (const char *) " + filename + "_array;\n\n")

    file.close()

    return

#==============================================================================
# Name : Traverse
#==============================================================================

def traverse(directory, output_h_file, output_cpp_file):

    for file in os.listdir(directory):

        file_path = os.path.join(directory, file)

        if (os.path.isfile(file_path) and not file.startswith('.')):
            print("Extracting binary data for file " + file_path)
            extract_binary_data(output_h_file, output_cpp_file, file_path)
        elif(os.path.isdir(file_path) and not file.startswith('.')):
            write_with_tab(output_h_file, "namespace " + file.lower() + "\n")
            write_with_tab(output_h_file, "{\n")
            write_with_tab(output_cpp_file, "namespace " + file.lower() + "\n")
            write_with_tab(output_cpp_file, "{\n")
            inc_tab()
            traverse(file_path, output_h_file, output_cpp_file)
            dec_tab()
            write_with_tab(output_cpp_file, "}\n")
            write_with_tab(output_h_file, "}\n")

    return

#==============================================================================
# Name : Main
#==============================================================================

args = parse_args()

tab_count = 0

# Creating Binary Data files to write

if os.path.exists(os.path.join(args.output, "KiwiApp_BinaryData.cpp")):
    os.remove(os.path.join(args.output, "KiwiApp_BinaryData.cpp"))
output_cpp_file = open(os.path.join(args.output, "KiwiApp_BinaryData.cpp"), "a")

if os.path.exists(os.path.join(args.output, "KiwiApp_BinaryData.h")):
    os.remove(os.path.join(args.output, "KiwiApp_BinaryData.h"))
output_h_file = open(os.path.join(args.output, "KiwiApp_BinaryData.h"), "a")

# Append license header info

header_file = open(os.path.join(os.getcwd(), "Resources", "SourceHeader.txt"), "r")

header_txt = header_file.read() + '\n';
header_file.close()

header_txt += "// ============================================================================\n"
header_txt += "// This is an auto-generated file: Any edits you make may be overwritten!\n"
header_txt += "// ============================================================================\n\n"
output_cpp_file.write(header_txt)
output_h_file.write(header_txt)

output_h_file.write("#pragma once\n\n")

write_with_tab(output_h_file, "namespace kiwi\n{\n")
write_with_tab(output_cpp_file, "namespace kiwi\n{\n")

inc_tab()
write_with_tab(output_cpp_file, "namespace binary_data\n")
write_with_tab(output_cpp_file, "{\n")
write_with_tab(output_h_file, "namespace binary_data\n")
write_with_tab(output_h_file, "{\n")

inc_tab()

traverse(args.input, output_h_file, output_cpp_file)

dec_tab()
write_with_tab(output_h_file, "}\n")
write_with_tab(output_cpp_file, "}\n")

dec_tab()
write_with_tab(output_h_file, "}\n")
write_with_tab(output_cpp_file, "}\n")

output_cpp_file.close()
output_h_file.close()

sys.exit(0)
