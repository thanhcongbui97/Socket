cmake_minimum_required(VERSION 3.10)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
add_executable(helloworld app.c)
target_link_libraries(helloworld Threads::Threads)

