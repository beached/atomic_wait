#Copyright( c ) Darrell Wright
#
#Distributed under the Boost Software License, Version 1.0.( See accompanying
#file LICENSE or copy at http: // www.boost.org/LICENSE_1_0.txt)
#
#Official repository : https: // github.com/beached/parallel
#

#Allows building all in some IDE's
add_custom_target(full)

add_library(daw_aw_test INTERFACE)
target_link_libraries(daw_aw_test INTERFACE daw::synchro ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(daw_aw_test INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/permissive->)

set(all_tests ${TEST_SOURCES})

add_executable( atomic_wait_tst src/atomic_wait_test.cpp )
target_link_libraries( atomic_wait_tst PRIVATE daw_test )
add_test( atomic_wait_test atomic_wait_tst )
add_dependencies( full atomic_wait_tst )

add_executable( sample src/sample.cpp src/sample.hpp )
target_link_libraries( sample PRIVATE daw_test )
add_test( sample_test sample )
add_dependencies( full sample )

