project(docs)

# artifacts folder should be valid http-server root with css inside for testing
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/styles.css"
               "${ARTIFACTS_DIR}/styles.css" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/doom-log.html"
               "${ARTIFACTS_DIR}/doom-log.html" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test_local.py"
               "${CMAKE_BINARY_DIR}/test_local.py" COPYONLY)

# xsltproc alternatives:
# altovaxml /validate /dtd schema.dtd /v $(bookscheme)
# altovaxml /validate /schema schema.xsd /v $(bookscheme)
# XML to HTM transformation
# java -jar "c:\xalan\xalan.jar" -HTML -IN $(bookscheme) -XSL $(transformator) -OUT $(target)
# xsltproc -o $(target) $(transformator) $(bookscheme)
# msxsl.exe -t $(bookscheme) $(transformator) | dos2unix > $(target)

set(full_book     "${CMAKE_CURRENT_SOURCE_DIR}/main.xml")
set(transformator "${CMAKE_CURRENT_SOURCE_DIR}/xhtml.xsl")
set(book_schema   "${CMAKE_CURRENT_SOURCE_DIR}/schema.xsd")
set(target        "${ARTIFACTS_DIR}/index.html")

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
    llvm.xml
    pal.xml
    physical_memory.xml
    pic.xml
    predication.xml
    preface.xml
    register_stack.xml
    virtual_memory.xml
)
# message(STATUS "XML sources: ${DOC_LIST}")

add_custom_command(
    OUTPUT ${target}
    COMMAND echo "xsltproc: ${target}"
    COMMAND xsltproc
        --stringparam PACKAGE_VERSION "${GIT_PACKAGE_VERSION}"
        --stringparam PACKAGE_DATE "${GIT_PACKAGE_DATE}"
        --stringparam PACKAGE_DATE_YEAR "${GIT_PACKAGE_DATE_YEAR}"
        -o ${target} ${transformator} ${full_book}

    DEPENDS ${DOC_LIST} ${transformator} ${full_book}
    COMMENT "Create docs xhtml index.html"
)

add_custom_target(${PROJECT_NAME} ALL DEPENDS ${target})

# XML validation requires w3c-sgml-lib DTD files
# MinGW hasn't offline SGML lib for XHTML checking,
# so don't verify xhtml on Windows
if(LINUX)
    # validate domain-specifiec XML sources
    foreach(xml_src ${DOC_LIST})
        # message(STATUS "xml file: ${xml_src}")
        add_custom_target(${xml_src}.dummy
            # PRE_BUILD
            COMMAND xmllint --schema ${book_schema} --nonet --noout --quiet ${CMAKE_CURRENT_SOURCE_DIR}/${xml_src}
            DEPENDS ${xml_src} ${book_schema}
            COMMENT "xmllint XML XSD schema validation: ${CMAKE_CURRENT_SOURCE_DIR}/${xml_src}"
        )
        add_dependencies(${PROJECT_NAME} ${xml_src}.dummy)
    endforeach(xml_src)
    # validate result as XHTML
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND xmllint --noout --nonet --quiet --valid ${target}
        COMMENT "xmllint XHTML validation: ${target}"
    )
endif()

# test single xml source
#isa.html: $(transformator) isa.xml $(sources)
# xsltproc -o isa.html $(transformator) isa.xml


####################################################################################################
# sitemap
####################################################################################################
list(APPEND SITEMAP_LIST
  dump.html
  gen.html
  index.html
  out_diz.s
  out_dump.s
  program.html
  doom-log.html
)

execute_process(COMMAND
    git -C "${CMAKE_CURRENT_SOURCE_DIR}" log -1 --format=%cd "--date=iso8601-strict"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_PACKAGE_DATE_ISO8601)
message(STATUS "GIT_PACKAGE_DATE_ISO8601 = ${GIT_PACKAGE_DATE_ISO8601}")

set(SITEMAP "${ARTIFACTS_DIR}/sitemap.xml")

file(WRITE ${SITEMAP}
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<urlset\n"
    "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "    xsi:schemaLocation=\"http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd\"\n"
    "    xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\"\n"
    ">\n")

set(SITE "https://bdpx.github.io/postrisc")

foreach(src ${SITEMAP_LIST})
    file(APPEND ${SITEMAP}
      "<url>\n"
      "  <loc>${SITE}/${src}</loc>\n"
      "  <lastmod>${GIT_PACKAGE_DATE_ISO8601}</lastmod>\n"
      "  <changefreq>hourly</changefreq>\n"
      "  <priority>0.9</priority>\n"
      "</url>\n")
endforeach(src)

file(APPEND ${SITEMAP} "</urlset>\n")
