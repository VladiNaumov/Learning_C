// sender.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hMapFile;  // Объявление дескриптора общей памяти
    int *pBuf;        // Указатель на данные в общей памяти
    int numbers[2];   // Числа, которые будут отправлены

    // Запрос ввода двух чисел
    printf("Enter two numbers: ");
    scanf("%d %d", &numbers[0], &numbers[1]);

    // Создание файла общей памяти для чисел
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,  // Используется системное файловое пространство
        NULL,                  // Без защиты по умолчанию
        PAGE_READWRITE,        // Разрешение на чтение и запись
        0,                     // Размер буфера, 0 - автоматический размер
        sizeof(int) * 2,       // Размер данных в байтах
        "NumbersSharedMemory"  // Имя объекта общей памяти
    );

    if (hMapFile == NULL) {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    // Отображение общей памяти в адресное пространство текущего процесса
    pBuf = (int *)MapViewOfFile(
        hMapFile,             // Дескриптор файла общей памяти
        FILE_MAP_ALL_ACCESS,  // Разрешение на чтение и запись
        0,                    // Смещение в файле (нулевое смещение)
        0,                    // Смещение в буфере (нулевое смещение)
        sizeof(int) * 2       // Размер данных для отображения
    );

    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Копирование чисел в общую память
    CopyMemory(pBuf, numbers, sizeof(int) * 2);

    // Закрытие отображения общей памяти
    UnmapViewOfFile(pBuf);

    // Закрытие дескриптора файла общей памяти
    CloseHandle(hMapFile);

    return 0;
}
