
#patch xml to replace ${NAME} entry points to real values
#expected env set before call:
# XML_SRC = path
# XML_DST = path
# TIMING_INFO = boolean
# VERBOSE_INFO = boolean
# ROM_IMAGE_PATH = path

message(STATUS "Configure patched xml config ${XML_DST} from ${XML_SRC}")

configure_file(${XML_SRC} "${XML_DST}")


