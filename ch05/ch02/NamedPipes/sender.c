// sender.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hPipe;    // Дескриптор именованного канала
    DWORD dwWritten; // Количество записанных байтов
    int numbers[2];  // Числа, которые будут отправлены

    // Запрос ввода двух чисел
    printf("Enter two numbers: ");
    scanf("%d %d", &numbers[0], &numbers[1]);

    // Создание именованного канала для отправки чисел
    hPipe = CreateFile(
        "\\\\.\\pipe\\NumbersPipe",  // Имя именованного канала
        GENERIC_WRITE,               // Разрешение на запись
        0,                           // Нет совместного доступа
        NULL,                        // Атрибуты по умолчанию
        OPEN_EXISTING,               // Открыть существующий канал
        0,                           // Флаги по умолчанию
        NULL                         // Дескриптор шаблона файлов
    );

    if (hPipe != INVALID_HANDLE_VALUE) {
        // Запись чисел в именованный канал
        WriteFile(hPipe, numbers, sizeof(numbers), &dwWritten, NULL);
        CloseHandle(hPipe);  // Закрытие дескриптора канала
    } else {
        printf("Could not open pipe.\n");
    }

    return 0;
}
