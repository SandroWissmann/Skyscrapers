cmake_minimum_required(VERSION 3.5)

project(SkyscrapersTest LANGUAGES CXX)


add_definitions(-DGTEST_LANGUAGE_CXX17)

find_package(Threads REQUIRED)

if ($ENV{GOOGLETEST_DIR})
    set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    if (NOT "/mnt/Programmierung/Cpp/_library/gtest/googletest/" STREQUAL "")
        message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    endif ()
    set(GOOGLETEST_DIR "/mnt/Programmierung/Cpp/_library/gtest/googletest/")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})

add_executable(
    SkyscrapersTest
    testdataprovider.h
    tst_permutationtest.h
    tst_backtrackingtest.h
    main.cpp
    ../Skyscrapers/permutation.cpp
    ../Skyscrapers/permutation/borderiterator.cpp
    ../Skyscrapers/permutation/nopes.cpp
    ../Skyscrapers/permutation/readdirection.cpp
    ../Skyscrapers/permutation/board.cpp
    ../Skyscrapers/permutation/field.cpp
    ../Skyscrapers/permutation/row.cpp
    ../Skyscrapers/permutation/cluepair.cpp
    ../Skyscrapers/permutation/permutations.cpp
    ../Skyscrapers/permutation/slice.cpp
    ../Skyscrapers/backtracking.cpp
    ../Skyscrapers/backtracking/nopes.cpp
    ../Skyscrapers/backtracking/board.cpp
    ../Skyscrapers/backtracking/rowdata.cpp

               ${GTestFiles})
add_test(NAME SkyscrapersTest COMMAND SkyscrapersTest)
target_link_libraries(SkyscrapersTest PRIVATE Threads::Threads)

