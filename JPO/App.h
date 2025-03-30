#ifndef APP_H
#define APP_H

#include <wx/wx.h>

/**
 * @file App.h
 * @brief Nag³ówek aplikacji wxWidgets.
 *
 * Plik zawiera deklaracjê klasy `MyApp`, która dziedziczy po klasie `wxApp`
 * i implementuje metodê `OnInit`, s³u¿¹c¹ do inicjalizacji aplikacji.
 */

 /**
  * @class MyApp
  * @brief Klasa reprezentuj¹ca aplikacjê wxWidgets.
  *
  * Klasa `MyApp` dziedziczy po klasie `wxApp` i implementuje metodê `OnInit`.
  * Jest odpowiedzialna za inicjalizacjê aplikacji oraz ustawienie g³ównego okna aplikacji.
  *
  * @see wxApp
  */
class MyApp : public wxApp {
public:
    /**
     * @brief Inicjalizacja aplikacji.
     *
     * Funkcja `OnInit` jest wywo³ywana przy starcie aplikacji. Tworzy g³ówne okno
     * aplikacji oraz ustawia jego w³aœciwoœci (np. rozmiar, centrowanie).
     *
     * @return Zwraca `true`, jeœli inicjalizacja aplikacji zakoñczy³a siê sukcesem,
     *         w przeciwnym razie zwraca `false`.
     */
    virtual bool OnInit();
};

#endif