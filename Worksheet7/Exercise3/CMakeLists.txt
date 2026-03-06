cmake_minimum_required(VERSION 3.19)
project(VRBaseStation LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools OpenGLWidgets)
find_package(VTK REQUIRED)
include_directories("C:/Program Files (x86)/VTK/include/vtk-9.6")

set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

qt_add_executable(VRBaseStation
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.h
    ModelPart.cpp
    ModelPartList.h
    ModelPartList.cpp
    icons.qrc
    optiondialog.cpp
    optiondialog.h
    optiondialog.ui
)

qt_add_translations(
    TARGETS VRBaseStation
    TS_FILES VRBaseStation_en_GB.ts
)

target_link_libraries(VRBaseStation
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGLWidgets
        ${VTK_LIBRARIES}

)

include(GNUInstallDirs)
install(TARGETS VRBaseStation
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET VRBaseStation
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Copy across OpenVR bindings that map controllers
# The program will expect to find these in the build dir when it runs
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/)