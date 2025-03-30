#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

/**
 * @file App.cpp
 * @brief Implementacja aplikacji wxWidgets.
 *
 * Plik zawiera implementacj� g��wnej aplikacji wxWidgets, kt�ra uruchamia g��wne okno
 * aplikacji i ustawia jej w�a�ciwo�ci.
 */

 /**
  * @brief Funkcja inicjalizuj�ca aplikacj�.
  *
  * Funkcja `OnInit` jest wywo�ywana przy uruchamianiu aplikacji. Tworzy g��wne okno
  * aplikacji (obiekt klasy `MainFrame`), ustawia jego rozmiar, centrowanie i wy�wietlanie.
  * Funkcja zwraca `true`, co oznacza, �e aplikacja zosta�a poprawnie zainicjalizowana.
  *
  * @return Zwraca `true`, je�li inicjalizacja aplikacji powiod�a si�, w przeciwnym razie `false`.
  */
bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("Jako�� powietrza w Polsce");  ///< Tworzenie g��wnego okna
    frame->SetClientSize(800, 800);                                  ///< Ustawienie rozmiaru okna
    frame->Center();                                                 ///< Centrowanie okna na ekranie
    frame->Show(true);                                               ///< Wy�wietlenie okna
    return true;                                                     ///< Zwr�cenie warto�ci true, aby aplikacja dzia�a�a
}