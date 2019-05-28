add_executable(lanczos_scaling lanczos_scaling.cpp)
target_link_libraries(lanczos_scaling
    PRIVATE
      gil_compile_options
      gil_include_directories
      gil_dependencies)

