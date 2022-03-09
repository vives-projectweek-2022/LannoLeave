add_library(helper INTERFACE)

target_include_directories(helper INTERFACE
  ./include
)

target_sources(helper INTERFACE
  helper_funcs_var.cpp
)

target_compile_definitions(helper INTERFACE
  $<$<CONFIG:Debug>:
    DEBUG=1
  >
)
