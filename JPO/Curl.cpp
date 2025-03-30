#include "Curl.h"
#include <curl/curl.h>
#include <iostream>

/**
 * @brief Funkcja callback do zapisu danych zwróconych przez CURL.
 *
 * Funkcja ta jest wywo³ywana przez bibliotekê libcurl, aby zapisaæ
 * dane odpowiedzi w postaci ci¹gu znaków do zmiennej typu std::string.
 *
 * @param contents WskaŸnik do danych, które maj¹ zostaæ zapisane.
 * @param size Rozmiar pojedynczego elementu danych.
 * @param nmemb Liczba elementów danych.
 * @param output WskaŸnik do zmiennej std::string, do której bêd¹ zapisywane dane.
 * @return Zwraca ca³kowity rozmiar zapisanych danych (size * nmemb).
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * @brief Funkcja wykonuj¹ca zapytanie HTTP za pomoc¹ biblioteki CURL.
 *
 * Funkcja ta inicjuje sesjê CURL, ustawia odpowiednie opcje (URL, funkcjê
 * callback do zapisu danych, wy³¹cza weryfikacjê SSL) i wykonuje zapytanie HTTP.
 * OdpowiedŸ serwera jest zapisywana w zmiennej response.
 *
 * @param url Adres URL, do którego zostanie wys³ane zapytanie.
 * @param response Referencja do zmiennej std::string, w której zostanie zapisany
 *                 wynik zapytania.
 * @return Zwraca true, jeœli zapytanie zosta³o wykonane pomyœlnie, w przeciwnym
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
 * Funkcja ta konwertuje odpowiedŸ w postaci ci¹gu znaków (response) do obiektu
 * typu Json::Value przy u¿yciu biblioteki JsonCpp. W przypadku b³êdu zwraca false.
 *
 * @param response OdpowiedŸ w formie ci¹gu znaków, która ma byæ sparsowana.
 * @param root Obiekt typu Json::Value, do którego zapisane zostan¹ dane.
 * @return Zwraca true, jeœli odpowiedŸ JSON zosta³a poprawnie sparsowana, w
 *         przeciwnym razie zwraca false.
 */
bool parseJsonResponse(const std::string& response, Json::Value& root) {
    Json::CharReaderBuilder reader;
    std::string errs;
    std::istringstream s(response);
    return Json::parseFromStream(reader, s, &root, &errs);
}