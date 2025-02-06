set(sources
  "main.c"
)


idf_component_register(
  SRCS "${sources}"
  INCLUDE_DIRS "."
  REQUIRES esp_mm esp_psram
)


target_compile_options(${COMPONENT_LIB} PRIVATE -fms-extensions)
