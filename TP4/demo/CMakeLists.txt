add_executable(simu_tp4 simu_tp4.cxx)

target_compile_definitions(simu_tp4 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(simu_tp4 particules)