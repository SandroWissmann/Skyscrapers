cmake_minimum_required(VERSION 3.5)

project(Skyscrapers LANGUAGES CXX)

add_executable(Skyscrapers
    shared/missingnumberinsequence
    shared/field.h
    shared/field.cpp
    shared/point.h
    shared/readdirection.h
    shared/readdirection.cpp
    shared/borderiterator.h
    shared/borderiterator.cpp
    shared/rowclues.h
    shared/rowclues.cpp
    shared/row.h
    shared/row.cpp
    shared/board.h
    shared/board.cpp
    permutation.h
    permutation.cpp
    permutation/span.h
    permutation/cluepair.h
    permutation/cluepair.cpp
    permutation/permutations.h
    permutation/permutations.cpp
    permutation/slice.h
    permutation/slice.cpp
    backtracking.h
    backtracking.cpp
    backtracking/algorithm.h
    backtracking/algorithm.cpp
    hybrid.h
    hybrid.cpp
    codewarsbacktracking.h
    codewarsbacktracking.cpp
    codewarspermutation.h
    codewarspermutation.cpp
    main.cpp
    )
