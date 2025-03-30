#ifndef APP_H
#define APP_H

#include <wx/wx.h>

/**
 * @file App.h
 * @brief Nag��wek aplikacji wxWidgets.
 *
 * Plik zawiera deklaracj� klasy `MyApp`, kt�ra dziedziczy po klasie `wxApp`
 * i implementuje metod� `OnInit`, s�u��c� do inicjalizacji aplikacji.
 */

 /**
  * @class MyApp
  * @brief Klasa reprezentuj�ca aplikacj� wxWidgets.
  *
  * Klasa `MyApp` dziedziczy po klasie `wxApp` i implementuje metod� `OnInit`.
  * Jest odpowiedzialna za inicjalizacj� aplikacji oraz ustawienie g��wnego okna aplikacji.
  *
  * @see wxApp
  */
class MyApp : public wxApp {
public:
    /**
     * @brief Inicjalizacja aplikacji.
     *
     * Funkcja `OnInit` jest wywo�ywana przy starcie aplikacji. Tworzy g��wne okno
     * aplikacji oraz ustawia jego w�a�ciwo�ci (np. rozmiar, centrowanie).
     *
     * @return Zwraca `true`, je�li inicjalizacja aplikacji zako�czy�a si� sukcesem,
     *         w przeciwnym razie zwraca `false`.
     */
    virtual bool OnInit();
};

#endif