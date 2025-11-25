#Declares current directory as Zephyr library (using directory name as lib name)
zephyr_library()

#List source code files for lib
zephyr_library_sources(max7219.c)

#Add header files to CMake search directories
zephyr_include_directories(.)