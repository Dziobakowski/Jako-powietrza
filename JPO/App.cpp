#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

/**
 * @file App.cpp
 * @brief Implementacja aplikacji wxWidgets.
 *
 * Plik zawiera implementacjê g³ównej aplikacji wxWidgets, która uruchamia g³ówne okno
 * aplikacji i ustawia jej w³aœciwoœci.
 */

 /**
  * @brief Funkcja inicjalizuj¹ca aplikacjê.
  *
  * Funkcja `OnInit` jest wywo³ywana przy uruchamianiu aplikacji. Tworzy g³ówne okno
  * aplikacji (obiekt klasy `MainFrame`), ustawia jego rozmiar, centrowanie i wyœwietlanie.
  * Funkcja zwraca `true`, co oznacza, ¿e aplikacja zosta³a poprawnie zainicjalizowana.
  *
  * @return Zwraca `true`, jeœli inicjalizacja aplikacji powiod³a siê, w przeciwnym razie `false`.
  */
bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("Jakoœæ powietrza w Polsce");  ///< Tworzenie g³ównego okna
    frame->SetClientSize(800, 800);                                  ///< Ustawienie rozmiaru okna
    frame->Center();                                                 ///< Centrowanie okna na ekranie
    frame->Show(true);                                               ///< Wyœwietlenie okna
    return true;                                                     ///< Zwrócenie wartoœci true, aby aplikacja dzia³a³a
}