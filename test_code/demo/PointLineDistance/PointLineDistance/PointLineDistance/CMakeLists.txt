file(GLOB_RECURSE SRCS "*.cpp" "*.h")

include_directories(.)

set(TARGET_NAME PointLineDistance)

SET(CMAKE_MFC_FLAG 1)
add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

add_executable(${TARGET_NAME} ${SRCS})
func_lxruntime_output(${TARGET_NAME} ${PRJ_BINDIR})

# LXTestCluster.cpp使用了kc内部头文件
target_include_directories(${TARGET_NAME} PRIVATE ${DEV_INC_KCACHE}/kc)

func_target_includedev(${TARGET_NAME})
func_target_linkdev(${TARGET_NAME})

func_target_includeice(${TARGET_NAME})
func_target_linkice(${TARGET_NAME})

func_target_includegridbase(${TARGET_NAME})
func_target_linkgridbase(${TARGET_NAME})

func_target_includemqbase(${TARGET_NAME})
func_target_linkmqbase(${TARGET_NAME})

func_target_includegrid(${TARGET_NAME})
func_target_linkgrid(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS") 