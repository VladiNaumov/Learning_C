cmake_minimum_required(VERSION 3.29)
project(Lion C)

set(CMAKE_C_STANDARD 99)

# Задает каталог вывода для бинарных файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Получаем имя файла без расширения
#get_filename_component(SOURCE_FILE "example" NAME_WE)

# Добавляем исполняемый файл с тем же именем, что и исходный файл
# add_executable(${SOURCE_FILE}  server/server.c)


# Добавляем исполняемый файл с тем же именем, что и исходный файл
add_executable(clion ch03/ch01/file/table_pointer_f_server.c)