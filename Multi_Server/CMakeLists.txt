cmake_minimum_required(VERSION 2.8.12)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(helloworld app.c)
target_link_libraries(helloworld Threads::Threads)

