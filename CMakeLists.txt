project(s.cluster)
add_executable(s.cluster Cluster.cpp StringUtils.cpp Configuration.cpp Status.cpp SelfHealth.cpp NetworkCheck.cpp Logger.cpp NodeCheck.cpp Types.h)
target_link_libraries(s.cluster pthread)
