#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>
#include <string>
#include <map>
#include "Curl.h"
#include "PlotPanel.h"  // Na pocz�tku pliku

/**
 * @brief G��wna klasa okna aplikacji, dziedziczy po `wxFrame`.
 *
 * Klasa `MainFrame` reprezentuje g��wne okno aplikacji, kt�re umo�liwia u�ytkownikowi
 * interakcj� z aplikacj� do analizy danych o jako�ci powietrza. Zawiera wszystkie
 * interfejsy u�ytkownika, takie jak pola tekstowe, listy, przyciski, oraz odpowiednie
 * metody do obs�ugi zdarze�.
 */
class MainFrame : public wxFrame {
public:
    /**
     * @brief Konstruktor klasy `MainFrame`.
     *
     * Inicjalizuje g��wne okno aplikacji, ustawia tytu� okna oraz inicjalizuje
     * wszystkie kontrolki (przyciski, pola tekstowe, listy itp.).
     *
     * @param title Tytu� g��wnego okna aplikacji.
     */
    MainFrame(const wxString& title);

private:
    // Kontrolki UI
    wxChoice* choice; ///< Lista rozwijana zawieraj�ca stacje.
    wxTextCtrl* cityInput; ///< Pole tekstowe do wpisania miasta.
    wxButton* filterButton; ///< Przycisk do filtrowania stacji.
    wxStaticText* label; ///< Etykieta nad list� rozwijan�.
    wxStaticText* infoLabel; ///< Etykieta wy�wietlaj�ca dodatkowe informacje.
    wxListBox* sensorList; ///< Lista stanowisk pomiarowych.
    wxTextCtrl* resultBox; ///< Pole tekstowe do wy�wietlania wynik�w analizy danych.
    wxChoice* startDateChoice; ///< Lista rozwijana do wyboru dolnej granicy czasowej.
    wxChoice* endDateChoice; ///< Lista rozwijana do wyboru g�rnej granicy czasowej.
    wxButton* confirmButton; ///< Przycisk do potwierdzenia wyboru zakresu czasowego.
    PlotPanel* plotPanel; ///< Panel wykresu, na kt�rym rysowane s� dane pomiarowe.

    // Struktury danych
    std::map<wxString, int> stationMap; ///< Mapa stacji i ich identyfikator�w.
    std::map<wxString, int> sensorMap; ///< Mapa stanowisk pomiarowych i ich identyfikator�w.
    std::vector<wxString> timestamps; ///< Przechowywanie wszystkich dost�pnych znacznik�w czasu.
    std::vector<std::pair<wxString, double>> values; ///< Wszystkie dane pomiarowe przed filtrowaniem.

    /**
     * @brief Pobiera dane o jako�ci powietrza z serwera.
     *
     * Ta metoda wykonuje ��danie HTTP do serwera, aby pobra� dane o jako�ci powietrza.
     * Po pobraniu danych uruchamiana jest analiza danych i ich wy�wietlenie.
     */
    void PobierzDane();

    /**
     * @brief Obs�uguje zdarzenie klikni�cia przycisku do filtrowania stacji.
     *
     * Ta metoda filtruje dost�pne stacje na podstawie wprowadzonego miasta i wy�wietla
     * je w li�cie rozwijanej.
     *
     * @param event Zdarzenie klikni�cia przycisku.
     */
    void FiltrujStacje(wxCommandEvent& event);

    /**
     * @brief Wy�wietla list� stanowisk pomiarowych dla wybranej stacji.
     *
     * Ta metoda generuje i wy�wietla list� stanowisk pomiarowych (czujnik�w) dla
     * wybranej stacji.
     *
     * @param event Zdarzenie klikni�cia przycisku wywo�uj�cego metod�.
     */
    void WyswietlStanowiska(wxCommandEvent& event);

    /**
     * @brief Pobiera dane pomiarowe dla wybranej stacji i analizuje je.
     *
     * Metoda ta pobiera dane o jako�ci powietrza dla wybranej stacji, a nast�pnie
     * wy�wietla wyniki analizy w polu tekstowym oraz rysuje wykres w panelu wykresu.
     *
     * @param event Zdarzenie klikni�cia przycisku wywo�uj�cego metod�.
     */
    void WyswietlDanePomiarowe(wxCommandEvent& event);

    /**
     * @brief Analizuje dane pomiarowe (min, max, �rednia, trend).
     *
     * Funkcja ta analizuje dane JSON zawieraj�ce informacje o jako�ci powietrza,
     * oblicza warto�ci minimalne, maksymalne, �rednie oraz trend w danych, a nast�pnie
     * wy�wietla je w odpowiednich polach.
     *
     * @param data Dane pomiarowe w formacie JSON.
     */
    void AnalizujDane(const Json::Value& data); ///< Analiza warto�ci min, max, �rednia, trend.

    /**
     * @brief Obs�uguje zdarzenie klikni�cia przycisku "Potwierd�".
     *
     * Ta metoda filtruje dane pomiarowe na podstawie wybranego zakresu czasowego
     * i aktualizuje wy�wietlane dane na wykresie oraz w polu tekstowym.
     * Je�li nie znaleziono �adnych danych w podanym zakresie, wy�wietlany jest komunikat
     * o braku danych, a wykres jest resetowany.
     *
     * @param event Zdarzenie klikni�cia przycisku "Potwierd�".
     */
    void PotwierdzZakres(wxCommandEvent& event);
};

#endif