// executor.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hMapFileNumbers, hMapFileOperation;  // Объявление дескрипторов общей памяти
    int *pNumbers;                             // Указатель на числа в общей памяти
    char *pOperation;                          // Указатель на операцию в общей памяти
    int result;                                // Результат вычислений

    // Открытие файла общей памяти для чисел
    hMapFileNumbers = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,      // Разрешение на чтение и запись
        FALSE,                    // Файл не открывается для наследования
        "NumbersSharedMemory"     // Имя объекта общей памяти
    );

    // Открытие файла общей памяти для операции
    hMapFileOperation = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,      // Разрешение на чтение и запись
        FALSE,                    // Файл не открывается для наследования
        "OperationSharedMemory"   // Имя объекта общей памяти
    );

    if (hMapFileNumbers == NULL || hMapFileOperation == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
        return 1;
    }

    // Отображение чисел из общей памяти в адресное пространство текущего процесса
    pNumbers = (int *)MapViewOfFile(
        hMapFileNumbers,         // Дескриптор файла общей памяти
        FILE_MAP_ALL_ACCESS,     // Разрешение на чтение и запись
        0,                       // Смещение в файле (нулевое смещение)
        0,                       // Смещение в буфере (нулевое смещение)
        sizeof(int) * 2          // Размер данных для отображения
    );

    // Отображение операции из общей памяти в адресное пространство текущего процесса
    pOperation = (char *)MapViewOfFile(
        hMapFileOperation,       // Дескриптор файла общей памяти
        FILE_MAP_ALL_ACCESS,     // Разрешение на чтение и запись
        0,                       // Смещение в файле (нулевое смещение)
        0,                       // Смещение в буфере (нулевое смещение)
        sizeof(char)             // Размер данных для отображения
    );

    if (pNumbers == NULL || pOperation == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFileNumbers);
        CloseHandle(hMapFileOperation);
        return 1;
    }

    // Выполнение операции в зависимости от указанного символа
    switch (*pOperation) {
        case '+':
            result = pNumbers[0] + pNumbers[1];
            break;
        case '-':
            result = pNumbers[0] - pNumbers[1];
            break;
        case '*':
            result = pNumbers[0] * pNumbers[1
