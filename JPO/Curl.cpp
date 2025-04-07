#include "Curl.h"
#include <curl/curl.h>
#include <iostream>

/**
 * @brief Funkcja callback do zapisu danych zwróconych przez CURL.
 *
 * Funkcja ta jest wywoływana przez bibliotekę libcurl, aby zapisać
 * dane odpowiedzi w postaci ciągu znaków do zmiennej typu std::string.
 *
 * @param contents Wskaźnik do danych, które mają zostać zapisane.
 * @param size Rozmiar pojedynczego elementu danych.
 * @param nmemb Liczba elementów danych.
 * @param output Wskaźnik do zmiennej std::string, do której będą zapisywane dane.
 * @return Zwraca całkowity rozmiar zapisanych danych (size * nmemb).
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * @brief Funkcja wykonująca zapytanie HTTP za pomocą biblioteki CURL.
 *
 * Funkcja ta inicjuje sesję CURL, ustawia odpowiednie opcje (URL, funkcję
 * callback do zapisu danych, wyłącza weryfikację SSL) i wykonuje zapytanie HTTP.
 * Odpowiedź serwera jest zapisywana w zmiennej response.
 *
 * @param url Adres URL, do którego zostanie wysłane zapytanie.
 * @param response Referencja do zmiennej std::string, w której zostanie zapisany
 *                 wynik zapytania.
 * @return Zwraca true, jeśli zapytanie zostało wykonane pomyślnie, w przeciwnym
 *         razie zwraca false.
 */
bool performCurlRequest(const std::string& url, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

/**
 * @brief Funkcja do parsowania odpowiedzi w formacie JSON.
 *
 * Funkcja ta konwertuje odpowiedź w postaci ciągu znaków (response) do obiektu
 * typu Json::Value przy użyciu biblioteki JsonCpp. W przypadku błędu zwraca false.
 *
 * @param response Odpowiedź w formie ciągu znaków, która ma być sparsowana.
 * @param root Obiekt typu Json::Value, do którego zapisane zostaną dane.
 * @return Zwraca true, jeśli odpowiedź JSON została poprawnie sparsowana, w
 *         przeciwnym razie zwraca false.
 */
bool parseJsonResponse(const std::string& response, Json::Value& root) {
    Json::CharReaderBuilder reader;
    std::string errs;
    std::istringstream s(response);
    return Json::parseFromStream(reader, s, &root, &errs);
}