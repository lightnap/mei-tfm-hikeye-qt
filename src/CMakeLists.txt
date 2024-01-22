add_subdirectory(common)
add_subdirectory(dataStructures)
add_subdirectory(graphics)
add_subdirectory(loadingModule)
add_subdirectory(utilities)

target_sources(${PROJECT_NAME} PRIVATE CMainWindow.cpp CMainWindow.hpp
                                       CMainWindow.ui main.cpp)
