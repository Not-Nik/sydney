function(add_naive_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} sydney)
    add_test(NAME ${name}
            COMMAND ${name})
endfunction()

add_naive_test(test_manual_coroutine)
add_naive_test(test_thread_pool)
add_naive_test(test_local_scheduler)
add_naive_test(test_spawn_blocking)
add_naive_test(test_task_switching)
add_naive_test(test_recursive_coroutine)
