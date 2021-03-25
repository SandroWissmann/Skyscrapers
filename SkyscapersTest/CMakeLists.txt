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
    tst_codewarspermutationtest.h
    tst_codewarsbacktrackingtest.h
    main.cpp
    ../Skyscrapers/shared/field.cpp
    ../Skyscrapers/shared/readdirection.cpp
    ../Skyscrapers/shared/borderiterator.cpp
    ../Skyscrapers/shared/rowclues.cpp
    ../Skyscrapers/shared/row.cpp
    ../Skyscrapers/shared/board.cpp
    ../Skyscrapers/permutation.cpp
    ../Skyscrapers/permutation/cluepair.cpp
    ../Skyscrapers/permutation/permutations.cpp
    ../Skyscrapers/permutation/slice.cpp
    ../Skyscrapers/backtracking.cpp
    ../Skyscrapers/backtracking/algorithm.cpp
    ../Skyscrapers/codewarsbacktracking.cpp
    ../Skyscrapers/codewarspermutation.cpp

               ${GTestFiles})
add_test(NAME SkyscrapersTest COMMAND SkyscrapersTest)
target_link_libraries(SkyscrapersTest PRIVATE Threads::Threads)

