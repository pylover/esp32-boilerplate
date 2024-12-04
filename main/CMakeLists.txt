set(sources
  "main.c"
)


idf_component_register(
  SRCS "${sources}"
  INCLUDE_DIRS "."
)


target_compile_options(${COMPONENT_LIB} PRIVATE -fms-extensions)
