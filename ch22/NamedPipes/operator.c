// operator.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hPipe;    // Дескриптор именованного канала
    DWORD dwWritten; // Количество записанных байтов
    char operation;  // Операция, которая будет отправлена

    // Запрос ввода операции
    printf("Enter operation (+, -, *): ");
    scanf(" %c", &operation);

    // Создание именованного канала для отправки операции
    hPipe = CreateFile(
        "\\\\.\\pipe\\OperatorPipe",  // Имя именованного канала
        GENERIC_WRITE,               // Разрешение на запись
        0,                           // Нет совместного доступа
        NULL,                        // Атрибуты по умолчанию
        OPEN_EXISTING,               // Открыть существующий канал
        0,                           // Флаги по умолчанию
        NULL                         // Дескриптор шаблона файлов
    );

    if (hPipe != INVALID_HANDLE_VALUE) {
        // Запись операции в именованный канал
        WriteFile(hPipe, &operation, sizeof(operation), &dwWritten, NULL);
        CloseHandle(hPipe);  // Закрытие дескриптора канала
    } else {
        printf("Could not open pipe.\n");
    }

    return 0;
}
