#include "Curl.h"
#include <curl/curl.h>
#include <iostream>

/**
 * @brief Funkcja callback do zapisu danych zwr�conych przez CURL.
 *
 * Funkcja ta jest wywo�ywana przez bibliotek� libcurl, aby zapisa�
 * dane odpowiedzi w postaci ci�gu znak�w do zmiennej typu std::string.
 *
 * @param contents Wska�nik do danych, kt�re maj� zosta� zapisane.
 * @param size Rozmiar pojedynczego elementu danych.
 * @param nmemb Liczba element�w danych.
 * @param output Wska�nik do zmiennej std::string, do kt�rej b�d� zapisywane dane.
 * @return Zwraca ca�kowity rozmiar zapisanych danych (size * nmemb).
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * @brief Funkcja wykonuj�ca zapytanie HTTP za pomoc� biblioteki CURL.
 *
 * Funkcja ta inicjuje sesj� CURL, ustawia odpowiednie opcje (URL, funkcj�
 * callback do zapisu danych, wy��cza weryfikacj� SSL) i wykonuje zapytanie HTTP.
 * Odpowied� serwera jest zapisywana w zmiennej response.
 *
 * @param url Adres URL, do kt�rego zostanie wys�ane zapytanie.
 * @param response Referencja do zmiennej std::string, w kt�rej zostanie zapisany
 *                 wynik zapytania.
 * @return Zwraca true, je�li zapytanie zosta�o wykonane pomy�lnie, w przeciwnym
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
 * Funkcja ta konwertuje odpowied� w postaci ci�gu znak�w (response) do obiektu
 * typu Json::Value przy u�yciu biblioteki JsonCpp. W przypadku b��du zwraca false.
 *
 * @param response Odpowied� w formie ci�gu znak�w, kt�ra ma by� sparsowana.
 * @param root Obiekt typu Json::Value, do kt�rego zapisane zostan� dane.
 * @return Zwraca true, je�li odpowied� JSON zosta�a poprawnie sparsowana, w
 *         przeciwnym razie zwraca false.
 */
bool parseJsonResponse(const std::string& response, Json::Value& root) {
    Json::CharReaderBuilder reader;
    std::string errs;
    std::istringstream s(response);
    return Json::parseFromStream(reader, s, &root, &errs);
}