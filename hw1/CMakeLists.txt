cmake_minimum_required(VERSION 3.17)
project(syspro-hw1 VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

add_executable(
        vaccine_monitor
        src/main.c
        src/util.c
        src/data.c
        src/hashtable.c
        src/bloomfilter.c
        src/skiplist.c
        src/hash.c
        include/util.h
        include/bloomfilter.h
        include/skiplist.h
        include/bitarray.h
        include/hashtable.h
        include/data.h
        include/hash.h src/commands.c include/commands.h src/handler.c include/handler.h)

add_executable(
        test_bit_array
        src/test/bitarray_test.c
        include/bitarray.h
)

add_executable(
        test_bloom_filter
        src/test/bloomfilter_test.c
        src/bloomfilter.c
        src/hash.c
        include/bloomfilter.h
        include/bitarray.h
        include/hash.h
)

add_executable(
        test_skip_list
        src/test/skiplist_test.c
        src/skiplist.c
        include/skiplist.h
)

add_executable(
        test_hash_table
        src/test/hashtable_test.c
        src/hashtable.c
        src/hash.c
        include/hashtable.h
        include/hash.h
)

target_link_libraries(vaccine_monitor -lm)

target_link_libraries(test_skip_list -lm)

target_link_libraries(test_hash_table -lm)

target_link_libraries(test_bloom_filter -lm -lcrypto)
