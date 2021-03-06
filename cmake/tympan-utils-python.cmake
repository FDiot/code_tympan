# Utilities related to python and cython

function(copy_and_install_python_files destination)
  file(
    COPY .
    DESTINATION "${PROJECT_BINARY_DIR}/${TYMPAN_CythonModules_Debug}/${destination}"
    FILES_MATCHING PATTERN "*.py" )
  file(
    COPY .
    DESTINATION "${PROJECT_BINARY_DIR}/${TYMPAN_CythonModules_Release}/${destination}"
    FILES_MATCHING PATTERN "*.py" )
  install(DIRECTORY .
    DESTINATION "${TYMPAN_CythonModules_Release}/${destination}"
    CONFIGURATIONS Release
    FILES_MATCHING PATTERN "*.py")
  install(DIRECTORY .
    DESTINATION "${TYMPAN_CythonModules_Debug}/${destination}"
    CONFIGURATIONS Debug
    FILES_MATCHING PATTERN "*.py")
endfunction()

function(path_relative_to_python outvar)
  file(RELATIVE_PATH relpath "${CMAKE_SOURCE_DIR}/python" "${CMAKE_CURRENT_SOURCE_DIR}")
  set(${outvar} "${relpath}" PARENT_SCOPE)
endfunction(path_relative_to_python)

function(configure_cython_module module destination)
  set_property(TARGET ${module} PROPERTY DEBUG_POSTFIX "")
  set_property(TARGET ${module} PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG
    "${CMAKE_BINARY_DIR}/${TYMPAN_CythonModules_Debug}/${destination}")
  set_property(TARGET ${module} PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE
    "${CMAKE_BINARY_DIR}/${TYMPAN_CythonModules_Release}/${destination}")
  # We do NOT want to depend on the debug version of the Python libs
endfunction()

function(install_cython_module module destination)
  install(TARGETS ${module}
    DESTINATION ${TYMPAN_CythonModules_Release}/${destination}
    CONFIGURATIONS Release)
  install(TARGETS ${module}
    DESTINATION ${TYMPAN_CythonModules_Debug}/${destination}
    CONFIGURATIONS Debug)  
endfunction()

# This function creates a new python test
function(add_python_test)
  set(options "")
  set(oneValueArgs "TARGET" "SCRIPT" "FOLDER")
  set(multiValueArgs "RUNTIME_PATH" "PYTHONPATH" "DEPS")
  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_test( 
    NAME ${_TARGET}
    # [WORKING_DIRECTORY dir]
    COMMAND ${PYTHON_EXECUTABLE} "${_SCRIPT}" -v 
    )
  _common_test_config()
  build_native_path_list(native_pythonpath "${_PYTHONPATH}")  
  set_property(TEST ${_TARGET} APPEND PROPERTY ENVIRONMENT "PYTHONPATH=${native_pythonpath}")
  
  if(_UNPARSED_ARGUMENTS)
    message(WARNING "add_python_test: unknown arguments remaining unparsed "
      "for target ${_TARGET}: " ${_UNPARSED_ARGUMENTS})
  endif()
endfunction()

set(CYTHON_PATCH_SCRIPT "${CMAKE_SOURCE_DIR}/tools/cython_patch.py"
  CACHE PATH "Where to find the cython_patch.py script ")
mark_as_advanced(CYTHON_PATCH_SCRIPT)

function(cython_patch)
  set(options "")
  set(oneValueArgs "OUTPUT" "FILE" )
  set(multiValueArgs "")
  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
  get_filename_component(dir   "${_FILE}" PATH )
  get_filename_component(fname "${_FILE}" NAME )
  set(patched_fname "${dir}/patched_${fname}")

  add_custom_command(OUTPUT ${patched_fname}
    COMMAND ${PYTHON_EXECUTABLE}
    ARGS ${CYTHON_PATCH_SCRIPT} "${_FILE}" "${patched_fname}" 
    WORKING_DIRECTORY ${_dir}
    DEPENDS "${_FILE}")


  if(_UNPARSED_ARGUMENTS)
    message(WARNING 
      "cython_patch: unknown arguments remaining unparsed: " 
      "${_UNPARSED_ARGUMENTS}")
  endif()
  set(${_OUTPUT} "${patched_fname}" PARENT_SCOPE)
endfunction()


