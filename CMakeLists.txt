cmake_minimum_required(VERSION 3.15)
project(DataBases C)

set(CMAKE_C_STANDARD 11)

add_executable(DataBases main.c dbRead.c dbRead.h dbStructures.h dbFilesHandler.c dbFilesHandler.h dbDeletion.c dbDeletion.h dbUpdate.c dbUpdate.h dbInsert.h dbInsert.c)