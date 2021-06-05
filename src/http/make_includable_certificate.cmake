
###################################################################
# generate includable certificates files
# Takes a file and embeds it in a C++ header as array of hex {}
###################################################################

if(NOT DEFINED INPUT_FILE)
  message(FATAL_ERROR "Required cmake variable INPUT_FILE not set!")
endif()
if(NOT DEFINED OUTPUT_FILE)
  message(FATAL_ERROR "Required cmake variable OUTPUT_FILE not set!")
endif()

if(NOT EXISTS ${INPUT_FILE})
   message(FATAL_ERROR "File '${INPUT_FILE}' does not exist!")
endif()

file(READ "${INPUT_FILE}" contents HEX)
file(WRITE "${OUTPUT_FILE}"  "/* generated hex string */\n")
file(APPEND "${OUTPUT_FILE}" "{\n")

string(REGEX MATCHALL ".." output "${contents}")
string(REGEX REPLACE ";" ",\n  0x" output "${output}")

file(APPEND "${OUTPUT_FILE}" "  0x${output}\n")
file(APPEND "${OUTPUT_FILE}" "}\n")
