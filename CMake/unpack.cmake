
# Unpack a file
# -----------------------------------------------------------------------------#
function(unpack_file package workingdirectory)
  if(APPLE)
    execute_process(COMMAND tar xzf ./${package} WORKING_DIRECTORY ${workingdirectory})
  elseif(UNIX)
    execute_process(COMMAND tar xzf ./${package} WORKING_DIRECTORY ${workingdirectory})
  elseif(WIN32)
    execute_process(COMMAND 7z x ${package} WORKING_DIRECTORY ${workingdirectory})
  endif()
endfunction(unpack_file)
