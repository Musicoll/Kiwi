

# The path to the JUCE modules.
set(JUCE_MODULES_PATH ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules)

# The function adds the sources for a juce module.
# MODULE_NAME is the name of your module (for example: juce_core)
function(add_juce_module SOURCES_FILE MODULE_NAME)
  file(GLOB MODULE_SOURCES
    ${JUCE_MODULES_PATH}/${MODULE_NAME}/*.cpp
    ${JUCE_MODULES_PATH}/${MODULE_NAME}/*.hpp
    ${JUCE_MODULES_PATH}/${MODULE_NAME}/*.c
    ${JUCE_MODULES_PATH}/${MODULE_NAME}/*.h
    ${JUCE_MODULES_PATH}/${MODULE_NAME}/*.mm)
  set(${SOURCES_FILE} ${${SOURCES_FILE}} ${MODULE_SOURCES} PARENT_SCOPE)
endfunction(add_juce_module)
