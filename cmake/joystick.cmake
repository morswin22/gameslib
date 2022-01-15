# Setup joystick library
set(JOYSTICK_DIR ${LIB_DIR}/joystick)

if (NOT EXISTS ${JOYSTICK_DIR})
  set(JOYSTICK_ARCHIVE_FILE_NAME joystick.zip)
  set(JOYSTICK_ARCHIVE_FILE_PATH ${LIB_DIR}/${JOYSTICK_ARCHIVE_FILE_NAME})

  message(STATUS "[JOYSTICK] Downloading to ${JOYSTICK_ARCHIVE_FILE_PATH}")
  message(STATUS "[JOYSTICK] Not yet existing: ${JOYSTICK_DIR}, Working directory: ${CMAKE_SOURCE_DIR}")
  file(DOWNLOAD https://github.com/drewnoakes/joystick/archive/master.zip ${JOYSTICK_ARCHIVE_FILE_PATH} SHOW_PROGRESS)

  if (NOT EXISTS ${JOYSTICK_ARCHIVE_FILE_PATH})
    message(FATAL_ERROR "Could not find ${JOYSTICK_ARCHIVE_FILE_PATH}")
  endif ()

  message(STATUS "[JOYSTICK] Unzipping '${JOYSTICK_ARCHIVE_FILE_NAME}' to '${LIB_DIR}'")
  execute_process(COMMAND unzip ${JOYSTICK_ARCHIVE_FILE_PATH} WORKING_DIRECTORY ${LIB_DIR} ERROR_VARIABLE tc_unzip)

  if (tc_unzip)
    message(FATAL_ERROR "[JOYSTICK] Could not unzip contents of the downloaded toolchain (${tc_unzip})")
  endif ()

  message(STATUS "[JOYSTICK] Moving '${LIB_DIR}/joystick-master/' to '${JOYSTICK_DIR}'")
  execute_process(COMMAND mv ${LIB_DIR}/joystick-master/ ${JOYSTICK_DIR} ERROR_VARIABLE tc_move)

  if (tc_move)
    message(FATAL_ERROR "[JOYSTICK] Could not perform move operation (${tc_move})")
  endif ()

  message(STATUS "[JOYSTICK] Removing joystick.zip from ${LIB_DIR}")
  execute_process(COMMAND rm ${JOYSTICK_ARCHIVE_FILE_PATH} ERROR_VARIABLE tc_remove ERROR_VARIABLE tc_remove)

  if (tc_remove)
    message(FATAL_ERROR "[JOYSTICK] Could not perform remove operation (${tc_remove})")
  endif ()
endif ()

if (NOT EXISTS ${JOYSTICK_DIR}/joystick.cc)
  message(FATAL_ERROR "[JOYSTICK] Could not locate joystick.cc file")
endif ()

file(GLOB JOYSTICK_SRC_FILES "${JOYSTICK_DIR}/joystick.cc")
file(GLOB JOYSTICK_HEADER_FILES "${JOYSTICK_DIR}/joystick.hh")
file(COPY ${JOYSTICK_HEADER_FILES} DESTINATION ${PROJECT_BINARY_DIR}/install/joystick/include)

set(JOYSTICK_INCLUDE_DIR ${PROJECT_BINARY_DIR}/install/joystick/include)
