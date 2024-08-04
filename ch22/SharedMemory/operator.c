// operator.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hMapFile;  // Объявление дескриптора общей памяти
    char *pBuf;       // Указатель на данные в общей памяти
    char operation;   // Операция, которая будет отправлена

    // Запрос ввода операции
    printf("Enter operation (+, -, *): ");
    scanf(" %c", &operation);

    // Создание файла общей памяти для операции
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,  // Используется системное файловое пространство
        NULL,                  // Без защиты по умолчанию
        PAGE_READWRITE,        // Разрешение на чтение и запись
        0,                     // Размер буфера, 0 - автоматический размер
        sizeof(char),          // Размер данных в байтах
        "OperationSharedMemory"  // Имя объекта общей памяти
    );

    if (hMapFile == NULL) {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    // Отображение общей памяти в адресное пространство текущего процесса
    pBuf = (char *)MapViewOfFile(
        hMapFile,             // Дескриптор файла общей памяти
        FILE_MAP_ALL_ACCESS,  // Разрешение на чтение и запись
        0,                    // Смещение в файле (нулевое смещение)
        0,                    // Смещение в буфере (нулевое смещение)
        sizeof(char)          // Размер данных для отображения
    );

    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Копирование операции в общую память
    CopyMemory(pBuf, &operation, sizeof(char));

    // Закрытие отображения общей памяти
    UnmapViewOfFile(pBuf);

    // Закрытие дескриптора файла общей памяти
    CloseHandle(hMapFile);

    return 0;
}
