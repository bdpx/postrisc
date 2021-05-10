cmake_minimum_required(VERSION 3.10)
project(docs)

# xsltproc alternatives:
# altovaxml /validate /dtd schema.dtd /v $(bookscheme)
# altovaxml /validate /schema schema.xsd /v $(bookscheme)
# XML to HTM transformation
# java -jar "c:\xalan\xalan.jar" -HTML -IN $(bookscheme) -XSL $(transformator) -OUT $(target)
# xsltproc -o $(target) $(transformator) $(bookscheme)
# msxsl.exe -t $(bookscheme) $(transformator) | dos2unix > $(target)

set(book_scheme   "${CMAKE_CURRENT_SOURCE_DIR}/main.xml")
SET(transformator "${CMAKE_CURRENT_SOURCE_DIR}/xhtml.xsl")
SET(target        "${CMAKE_CURRENT_SOURCE_DIR}/index.html")

list(APPEND DOC_LIST
    abi.xml
    cpu.xml
    debug.xml
    exceptions.xml
    extended.xml
    fpu.xml
    interrupts.xml
    intro.xml
    isa.xml
    pal.xml
    physical_memory.xml
    pic.xml
    predication.xml
    preface.xml
    register_stack.xml
    virtual_memory.xml
)
# message(STATUS "XML sources: ${DOC_LIST}")

ADD_CUSTOM_COMMAND(
    OUTPUT ${target}
    COMMAND xsltproc -o ${target} ${transformator} ${book_scheme}
    DEPENDS ${DOC_LIST} ${transformator} ${book_scheme}
    COMMENT "Create docs xhtml index.html"
)

add_custom_target(${PROJECT_NAME} ALL DEPENDS ${target})

# test single xml source
#isa.html: $(transformator) isa.xml $(sources)
# xsltproc -o isa.html $(transformator) isa.xml
