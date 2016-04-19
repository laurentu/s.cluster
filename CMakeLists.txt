project(s.cluster)
add_executable(s.cluster cluster.cpp StringUtils.cpp Configuration.cpp Status.cpp SelfHealth.cpp logger.cpp)
target_link_libraries(s.cluster pthread)
