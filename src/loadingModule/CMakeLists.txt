add_subdirectory(resourceLoader)
add_subdirectory(resourceLoaderFactory)

target_sources(${PROJECT_NAME} PRIVATE CLoadingModule.cpp CLoadingModule.hpp)
