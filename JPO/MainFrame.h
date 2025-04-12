#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <mutex>
#include "Curl.h"
#include "PlotPanel.h"

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
    wxButton* saveButton; ///< Przycisk do zapisywania danych do pliku JSON.
    wxButton* loadButton; ///< Przycisk do ³adowania danych z pliku JSON.
    wxStaticText* label; ///< Etykieta nad list¹ rozwijan¹.
    wxStaticText* infoLabel; ///< Etykieta wyœwietlaj¹ca dodatkowe informacje.
    wxListBox* sensorList; ///< Lista stanowisk pomiarowych.
    wxTextCtrl* resultBox; ///< Pole tekstowe do wyœwietlania wyników analizy danych.
    wxChoice* startDateChoice; ///< Lista rozwijana do wyboru dolnej granicy czasowej.
    wxChoice* endDateChoice; ///< Lista rozwijana do wyboru górnej granicy czasowej.
    wxButton* confirmButton; ///< Przycisk do potwierdzenia wyboru zakresu czasowego.
    PlotPanel* plotPanel; ///< Panel wykresu, na którym rysowane s¹ dane pomiarowe.
    wxGauge* progressGauge; ///< WskaŸnik postêpu dla operacji w tle.

    // Struktury danych
    std::map<wxString, int> stationMap; ///< Mapa stacji i ich identyfikatorów.
    std::map<wxString, int> sensorMap; ///< Mapa stanowisk pomiarowych i ich identyfikatorów.
    std::vector<wxString> timestamps; ///< Przechowywanie wszystkich dostêpnych znaczników czasu.
    std::vector<std::pair<wxString, double>> values; ///< Wszystkie dane pomiarowe przed filtrowaniem.
    Json::Value currentSensorData; ///< Aktualne dane pomiarowe w formacie JSON.

    // Muteksy do synchronizacji dostêpu do danych wspó³dzielonych
    std::mutex stationMapMutex; ///< Muteks dla mapy stacji.
    std::mutex sensorMapMutex; ///< Muteks dla mapy sensorów.
    std::mutex valuesMutex; ///< Muteks dla wartoœci pomiarowych.

    // Flagi stanu operacji
    bool stationsLoading; ///< Flaga informuj¹ca czy stacje s¹ w trakcie ³adowania.
    bool sensorsLoading; ///< Flaga informuj¹ca czy sensory s¹ w trakcie ³adowania.
    bool measurementsLoading; ///< Flaga informuj¹ca czy pomiary s¹ w trakcie ³adowania.

    /**
     * @brief Pobiera dane o jakoœci powietrza z serwera.
     *
     * Ta metoda wykonuje ¿¹danie HTTP do serwera, aby pobraæ dane o jakoœci powietrza.
     * Po pobraniu danych uruchamiana jest analiza danych i ich wyœwietlenie.
     * W przypadku braku dostêpu do internetu, próbuje za³adowaæ dane z pliku lokalnego.
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

    /**
     * @brief Zapisuje dane pomiarowe do pliku JSON.
     *
     * Ta metoda zapisuje aktualne dane pomiarowe do pliku JSON w katalogu aplikacji.
     * Nazwa pliku zawiera datê i czas pobrania danych.
     *
     * @param event Zdarzenie klikniêcia przycisku "Zapisz".
     */
    void ZapiszDane(wxCommandEvent& event);

    /**
     * @brief £aduje dane pomiarowe z pliku JSON.
     *
     * Ta metoda ³aduje historyczne dane pomiarowe z pliku JSON i wyœwietla je
     * w aplikacji.
     *
     * @param event Zdarzenie klikniêcia przycisku "Za³aduj".
     */
    void ZaladujDane(wxCommandEvent& event);

    /**
     * @brief Generuje nazwê pliku JSON na podstawie aktualnej daty i czasu.
     *
     * @return wxString Nazwa pliku z rozszerzeniem JSON.
     */
    wxString GenerujNazwePliku();
};

#endif