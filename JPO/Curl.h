#ifndef CURL_H
#define CURL_H

#include <string>
#include <json/json.h>

/**
 * @file Curl.h
 * @brief Deklaracja funkcji do obs�ugi zapyta� HTTP oraz parsowania odpowiedzi w formacie JSON.
 *
 * Plik nag��wkowy zawiera deklaracje funkcji wykorzystywanych do wykonywania zapyta� HTTP
 * z wykorzystaniem biblioteki CURL oraz parsowania odpowiedzi w formacie JSON.
 */

 /**
  * @brief Wykonuje zapytanie HTTP do podanego URL.
  *
  * Funkcja ta wykorzystuje bibliotek� CURL do wysy�ania zapyta� HTTP na okre�lony adres URL.
  * Odpowied� serwera jest przechowywana w zmiennej przekazanej jako argument.
  * Funkcja wy��cza weryfikacj� SSL.
  *
  * @param url Adres URL, do kt�rego wysy�ane jest zapytanie HTTP.
  * @param response Zmienna, do kt�rej zostan� zapisane dane zwr�cone przez serwer.
  * @return Zwraca `true`, je�li zapytanie zako�czy�o si� sukcesem, w przeciwnym razie `false`.
  */
bool performCurlRequest(const std::string& url, std::string& response);

/**
 * @brief Parsuje odpowied� w formacie JSON.
 *
 * Funkcja ta przyjmuje odpowied� serwera w formie ci�gu znak�w i parsuje j� do obiektu
 * typu `Json::Value`. Biblioteka JsonCpp jest wykorzystywana do konwersji odpowiedzi w
 * format JSON.
 *
 * @param response Odpowied� serwera w formacie JSON w postaci ci�gu znak�w.
 * @param root Obiekt typu `Json::Value`, do kt�rego zapisane zostan� dane z odpowiedzi.
 * @return Zwraca `true`, je�li odpowied� zosta�a poprawnie sparsowana, w przeciwnym razie `false`.
 */
bool parseJsonResponse(const std::string& response, Json::Value& root);

#endif