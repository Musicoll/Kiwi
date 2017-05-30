#--------------------------------------
# source_group_rec
#--------------------------------------

macro(source_group_rec SRC_FILES BEGIN_DIR)
    foreach(file ${SRC_FILES})
        file(RELATIVE_PATH relative_file "${BEGIN_DIR}" ${file})
        get_filename_component(relative_path ${relative_file} DIRECTORY)
        if("${relative_path}" STREQUAL "")
            source_group(Source FILES ${file})
        else()
            string(REPLACE "/" "\\\\" group ${relative_path})
            source_group("Source\\${group}" FILES ${file})
        endif()
    endforeach(file)
endmacro(source_group_rec)