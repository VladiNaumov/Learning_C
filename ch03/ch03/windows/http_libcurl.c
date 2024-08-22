#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void perform_get_request(CURL *curl) {
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/api/resource");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res));
}

void perform_post_request(CURL *curl) {
    CURLcode res;
    const char *data = "{\"name\":\"John\", \"age\":30}";

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/api/resource");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));
}

void perform_put_request(CURL *curl) {
    CURLcode res;
    const char *data = "{\"name\":\"John\", \"age\":31}";

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/api/resource/1");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        fprintf(stderr, "PUT request failed: %s\n", curl_easy_strerror(res));
}

void perform_delete_request(CURL *curl) {
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/api/resource/1");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        fprintf(stderr, "DELETE request failed: %s\n", curl_easy_strerror(res));
}

int main(void) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // GET запрос
        perform_get_request(curl);

        // POST запрос
        perform_post_request(curl);

        // PUT запрос
        perform_put_request(curl);

        // DELETE запрос
        perform_delete_request(curl);

        // Очистка
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
