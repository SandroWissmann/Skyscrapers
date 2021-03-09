cmake_minimum_required(VERSION 3.5)

project(Skyscrapers LANGUAGES CXX)

add_executable(Skyscrapers
    permutation.h
    permutation.cpp
    permutation/nopes.h
    permutation/nopes.cpp
    permutation/span.h
    permutation/point.h
    permutation/readdirection.h
    permutation/readdirection.cpp
    permutation/borderiterator.h
    permutation/borderiterator.cpp
    permutation/board.h
    permutation/board.cpp
    permutation/field.h
    permutation/field.cpp
    permutation/row.h
    permutation/row.cpp
    permutation/cluepair.h
    permutation/cluepair.cpp
    permutation/permutations.h
    permutation/permutations.cpp
    permutation/fieldelements.h
    permutation/slice.h
    permutation/slice.cpp
    backtracking.h
    backtracking.cpp
    backtracking/nopes.h
    backtracking/nopes.cpp
    backtracking/board.h
    backtracking/board.cpp
    backtracking/cluehints.h
    backtracking/cluehints.cpp
    main.cpp
    )
