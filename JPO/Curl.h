#ifndef CURL_H
#define CURL_H

#include <string>
#include <json/json.h>

/**
 * @file Curl.h
 * @brief Deklaracja funkcji do obs³ugi zapytañ HTTP oraz parsowania odpowiedzi w formacie JSON.
 *
 * Plik nag³ówkowy zawiera deklaracje funkcji wykorzystywanych do wykonywania zapytañ HTTP
 * z wykorzystaniem biblioteki CURL oraz parsowania odpowiedzi w formacie JSON.
 */

 /**
  * @brief Wykonuje zapytanie HTTP do podanego URL.
  *
  * Funkcja ta wykorzystuje bibliotekê CURL do wysy³ania zapytañ HTTP na okreœlony adres URL.
  * OdpowiedŸ serwera jest przechowywana w zmiennej przekazanej jako argument.
  * Funkcja wy³¹cza weryfikacjê SSL.
  *
  * @param url Adres URL, do którego wysy³ane jest zapytanie HTTP.
  * @param response Zmienna, do której zostan¹ zapisane dane zwrócone przez serwer.
  * @return Zwraca `true`, jeœli zapytanie zakoñczy³o siê sukcesem, w przeciwnym razie `false`.
  */
bool performCurlRequest(const std::string& url, std::string& response);

/**
 * @brief Parsuje odpowiedŸ w formacie JSON.
 *
 * Funkcja ta przyjmuje odpowiedŸ serwera w formie ci¹gu znaków i parsuje j¹ do obiektu
 * typu `Json::Value`. Biblioteka JsonCpp jest wykorzystywana do konwersji odpowiedzi w
 * format JSON.
 *
 * @param response OdpowiedŸ serwera w formacie JSON w postaci ci¹gu znaków.
 * @param root Obiekt typu `Json::Value`, do którego zapisane zostan¹ dane z odpowiedzi.
 * @return Zwraca `true`, jeœli odpowiedŸ zosta³a poprawnie sparsowana, w przeciwnym razie `false`.
 */
bool parseJsonResponse(const std::string& response, Json::Value& root);

#endif