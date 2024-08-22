#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#pragma comment(lib, "winhttp.lib")

void perform_http_request(LPCWSTR verb, LPCWSTR server, LPCWSTR path, LPCWSTR headers, LPCWSTR data) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;

    // Создание сессии
    hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession)
        hConnect = WinHttpConnect(hSession, server,
                                  INTERNET_DEFAULT_HTTP_PORT, 0);

    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, verb, path,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      0);

    if (hRequest) {
        BOOL results;
        if (wcscmp(verb, L"POST") == 0 || wcscmp(verb, L"PUT") == 0) {
            results = WinHttpSendRequest(hRequest,
                                         headers, (DWORD) -1,
                                         (LPVOID)data, (DWORD)wcslen(data) * sizeof(wchar_t),
                                         (DWORD)wcslen(data) * sizeof(wchar_t),
                                         0);
        } else {
            results = WinHttpSendRequest(hRequest,
                                         headers, (DWORD) -1,
                                         WINHTTP_NO_REQUEST_DATA, 0,
                                         0, 0);
        }

        if (results)
            WinHttpReceiveResponse(hRequest, NULL);

        if (results) {
            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            LPSTR pszOutBuffer;
            BOOL  bResults = FALSE;

            do {
                // Определение размера данных
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    printf("Error %u in WinHttpQueryDataAvailable.\n",
                           GetLastError());
                }

                pszOutBuffer = (LPSTR)malloc(dwSize + 1);
                if (!pszOutBuffer) {
                    printf("Out of memory\n");
                    dwSize = 0;
                } else {
                    ZeroMemory(pszOutBuffer, dwSize + 1);

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                                         dwSize, &dwDownloaded)) {
                        printf("Error %u in WinHttpReadData.\n", GetLastError());
                    } else {
                        printf("%s", pszOutBuffer);
                    }

                    free(pszOutBuffer);
                }
            } while (dwSize > 0);
        } else {
            printf("Error %d has occurred.\n", GetLastError());
        }

        WinHttpCloseHandle(hRequest);
    }

    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
}

int main() {
    // GET запрос
    perform_http_request(L"GET", L"example.com", L"/api/resource", WINHTTP_NO_ADDITIONAL_HEADERS, NULL);

    // POST запрос
    perform_http_request(L"POST", L"example.com", L"/api/resource",
                         L"Content-Type: application/json\r\n",
                         L"{\"name\":\"John\", \"age\":30}");

    // PUT запрос
    perform_http_request(L"PUT", L"example.com", L"/api/resource/1",
                         L"Content-Type: application/json\r\n",
                         L"{\"name\":\"John\", \"age\":31}");

    // DELETE запрос
    perform_http_request(L"DELETE", L"example.com", L"/api/resource/1", WINHTTP_NO_ADDITIONAL_HEADERS, NULL);

    return 0;
}

/*
 * Описание:
WinHttpOpen: Создает сессию для использования WinHTTP.
WinHttpConnect: Открывает сессию подключения к HTTP-серверу.
WinHttpOpenRequest: Подготавливает запрос для отправки на сервер.
WinHttpSendRequest: Отправляет запрос на сервер.
WinHttpReceiveResponse: Ожидает ответа от сервера.
WinHttpQueryDataAvailable и WinHttpReadData: Используются для чтения ответа сервера.

Зависимости:
Программа требует подключения библиотеки winhttp.lib, которая включена в состав Windows SDK. Скомпилировать можно с помощью Visual Studio или через командную строку cl:

bash
Копировать код
cl /W4 /EHsc /DUNICODE your_program.c winhttp.lib
Примечания:
Программа работает с Unicode (широкими символами), поэтому строки должны быть в формате L"...".
Для реальных приложений используйте безопасные методы работы с памятью и обязательно обрабатывайте возможные ошибки более тщательно.
Этот код позволяет отправлять основные HTTP-запросы на указанный сервер и получать ответы, отображая их на экране.
 *
 * */