#ifndef PLOT_PANEL_H
#define PLOT_PANEL_H

#include <wx/wx.h>
#include <vector>

/**
 * @file PlotPanel.h
 * @brief Nagłówek panelu do wyświetlania wykresu.
 *
 * Plik zawiera deklarację klasy `PlotPanel`, która dziedziczy po klasie `wxPanel`
 * i jest odpowiedzialna za wyświetlanie wykresu na panelu w aplikacji wxWidgets.
 */

 /**
  * @class PlotPanel
  * @brief Klasa reprezentująca panel wykresu.
  *
  * Klasa `PlotPanel` dziedziczy po klasie `wxPanel` i odpowiada za rysowanie wykresu
  * na panelu. Zawiera metody umożliwiające ustawianie danych wykresu oraz jego rysowanie
  * w odpowiedzi na zdarzenia związane z malowaniem.
  */
class PlotPanel : public wxPanel {
public:
    /**
     * @brief Konstruktor klasy `PlotPanel`.
     *
     * Tworzy panel wykresu w aplikacji. Inicjalizuje komponenty panelu, w tym
     * rejestruje zdarzenia rysowania wykresu.
     *
     * @param parent Okno, w którym panel będzie osadzony.
     */
    PlotPanel(wxWindow* parent);

    /**
     * @brief Ustawia dane wykresu.
     *
     * Funkcja ta ustawia dane wykresu na podstawie podanych par czas-wartość.
     * Dane są przechowywane w wektorze i używane do rysowania wykresu.
     *
     * @param newData Dane wykresu w postaci wektora par (czas, wartość).
     */
    void SetData(const std::vector<std::pair<wxString, double>>& newData);

    /**
     * @brief Funkcja malująca wykres.
     *
     * Funkcja ta jest wywoływana przy każdym zdarzeniu malowania. Odpowiada
     * za renderowanie wykresu na panelu na podstawie danych dostarczonych przez
     * metodę `SetData`.
     *
     * @param event Zdarzenie malowania (dostarczane przez wxWidgets).
     */
    void OnPaint(wxPaintEvent& event);

private:
    /// Przechowuje dane wykresu w postaci par czas-wartość.
    std::vector<std::pair<wxString, double>> data;

    /// Deklaracja tabeli zdarzeń dla tej klasy.
    DECLARE_EVENT_TABLE()
};

#endif