#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>
#include <string>
#include <map>
#include "Curl.h"
#include "PlotPanel.h"  // Na pocz¹tku pliku

/**
 * @brief G³ówna klasa okna aplikacji, dziedziczy po `wxFrame`.
 *
 * Klasa `MainFrame` reprezentuje g³ówne okno aplikacji, które umo¿liwia u¿ytkownikowi
 * interakcjê z aplikacj¹ do analizy danych o jakoœci powietrza. Zawiera wszystkie
 * interfejsy u¿ytkownika, takie jak pola tekstowe, listy, przyciski, oraz odpowiednie
 * metody do obs³ugi zdarzeñ.
 */
class MainFrame : public wxFrame {
public:
    /**
     * @brief Konstruktor klasy `MainFrame`.
     *
     * Inicjalizuje g³ówne okno aplikacji, ustawia tytu³ okna oraz inicjalizuje
     * wszystkie kontrolki (przyciski, pola tekstowe, listy itp.).
     *
     * @param title Tytu³ g³ównego okna aplikacji.
     */
    MainFrame(const wxString& title);

private:
    // Kontrolki UI
    wxChoice* choice; ///< Lista rozwijana zawieraj¹ca stacje.
    wxTextCtrl* cityInput; ///< Pole tekstowe do wpisania miasta.
    wxButton* filterButton; ///< Przycisk do filtrowania stacji.
    wxStaticText* label; ///< Etykieta nad list¹ rozwijan¹.
    wxStaticText* infoLabel; ///< Etykieta wyœwietlaj¹ca dodatkowe informacje.
    wxListBox* sensorList; ///< Lista stanowisk pomiarowych.
    wxTextCtrl* resultBox; ///< Pole tekstowe do wyœwietlania wyników analizy danych.
    wxChoice* startDateChoice; ///< Lista rozwijana do wyboru dolnej granicy czasowej.
    wxChoice* endDateChoice; ///< Lista rozwijana do wyboru górnej granicy czasowej.
    wxButton* confirmButton; ///< Przycisk do potwierdzenia wyboru zakresu czasowego.
    PlotPanel* plotPanel; ///< Panel wykresu, na którym rysowane s¹ dane pomiarowe.

    // Struktury danych
    std::map<wxString, int> stationMap; ///< Mapa stacji i ich identyfikatorów.
    std::map<wxString, int> sensorMap; ///< Mapa stanowisk pomiarowych i ich identyfikatorów.
    std::vector<wxString> timestamps; ///< Przechowywanie wszystkich dostêpnych znaczników czasu.
    std::vector<std::pair<wxString, double>> values; ///< Wszystkie dane pomiarowe przed filtrowaniem.

    /**
     * @brief Pobiera dane o jakoœci powietrza z serwera.
     *
     * Ta metoda wykonuje ¿¹danie HTTP do serwera, aby pobraæ dane o jakoœci powietrza.
     * Po pobraniu danych uruchamiana jest analiza danych i ich wyœwietlenie.
     */
    void PobierzDane();

    /**
     * @brief Obs³uguje zdarzenie klikniêcia przycisku do filtrowania stacji.
     *
     * Ta metoda filtruje dostêpne stacje na podstawie wprowadzonego miasta i wyœwietla
     * je w liœcie rozwijanej.
     *
     * @param event Zdarzenie klikniêcia przycisku.
     */
    void FiltrujStacje(wxCommandEvent& event);

    /**
     * @brief Wyœwietla listê stanowisk pomiarowych dla wybranej stacji.
     *
     * Ta metoda generuje i wyœwietla listê stanowisk pomiarowych (czujników) dla
     * wybranej stacji.
     *
     * @param event Zdarzenie klikniêcia przycisku wywo³uj¹cego metodê.
     */
    void WyswietlStanowiska(wxCommandEvent& event);

    /**
     * @brief Pobiera dane pomiarowe dla wybranej stacji i analizuje je.
     *
     * Metoda ta pobiera dane o jakoœci powietrza dla wybranej stacji, a nastêpnie
     * wyœwietla wyniki analizy w polu tekstowym oraz rysuje wykres w panelu wykresu.
     *
     * @param event Zdarzenie klikniêcia przycisku wywo³uj¹cego metodê.
     */
    void WyswietlDanePomiarowe(wxCommandEvent& event);

    /**
     * @brief Analizuje dane pomiarowe (min, max, œrednia, trend).
     *
     * Funkcja ta analizuje dane JSON zawieraj¹ce informacje o jakoœci powietrza,
     * oblicza wartoœci minimalne, maksymalne, œrednie oraz trend w danych, a nastêpnie
     * wyœwietla je w odpowiednich polach.
     *
     * @param data Dane pomiarowe w formacie JSON.
     */
    void AnalizujDane(const Json::Value& data); ///< Analiza wartoœci min, max, œrednia, trend.

    /**
     * @brief Obs³uguje zdarzenie klikniêcia przycisku "PotwierdŸ".
     *
     * Ta metoda filtruje dane pomiarowe na podstawie wybranego zakresu czasowego
     * i aktualizuje wyœwietlane dane na wykresie oraz w polu tekstowym.
     * Jeœli nie znaleziono ¿adnych danych w podanym zakresie, wyœwietlany jest komunikat
     * o braku danych, a wykres jest resetowany.
     *
     * @param event Zdarzenie klikniêcia przycisku "PotwierdŸ".
     */
    void PotwierdzZakres(wxCommandEvent& event);
};

#endif