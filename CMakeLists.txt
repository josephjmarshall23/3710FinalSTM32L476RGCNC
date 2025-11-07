cmake_minimum_required(VERSION 3.5)

project(PenPlotter LANGUAGES C)

add_executable(PenPlotter main.c)

include(GNUInstallDirs)
install(TARGETS PenPlotter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
