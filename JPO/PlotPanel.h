#ifndef PLOT_PANEL_H
#define PLOT_PANEL_H

#include <wx/wx.h>
#include <vector>

/**
 * @file PlotPanel.h
 * @brief Nag��wek panelu do wy�wietlania wykresu.
 *
 * Plik zawiera deklaracj� klasy `PlotPanel`, kt�ra dziedziczy po klasie `wxPanel`
 * i jest odpowiedzialna za wy�wietlanie wykresu na panelu w aplikacji wxWidgets.
 */

 /**
  * @class PlotPanel
  * @brief Klasa reprezentuj�ca panel wykresu.
  *
  * Klasa `PlotPanel` dziedziczy po klasie `wxPanel` i odpowiada za rysowanie wykresu
  * na panelu. Zawiera metody umo�liwiaj�ce ustawianie danych wykresu oraz jego rysowanie
  * w odpowiedzi na zdarzenia zwi�zane z malowaniem.
  */
class PlotPanel : public wxPanel {
public:
    /**
     * @brief Konstruktor klasy `PlotPanel`.
     *
     * Tworzy panel wykresu w aplikacji. Inicjalizuje komponenty panelu, w tym
     * rejestruje zdarzenia rysowania wykresu.
     *
     * @param parent Okno, w kt�rym panel b�dzie osadzony.
     */
    PlotPanel(wxWindow* parent);

    /**
     * @brief Ustawia dane wykresu.
     *
     * Funkcja ta ustawia dane wykresu na podstawie podanych par czas-warto��.
     * Dane s� przechowywane w wektorze i u�ywane do rysowania wykresu.
     *
     * @param newData Dane wykresu w postaci wektora par (czas, warto��).
     */
    void SetData(const std::vector<std::pair<wxString, double>>& newData);

    /**
     * @brief Funkcja maluj�ca wykres.
     *
     * Funkcja ta jest wywo�ywana przy ka�dym zdarzeniu malowania. Odpowiada
     * za renderowanie wykresu na panelu na podstawie danych dostarczonych przez
     * metod� `SetData`.
     *
     * @param event Zdarzenie malowania (dostarczane przez wxWidgets).
     */
    void OnPaint(wxPaintEvent& event);

private:
    /// Przechowuje dane wykresu w postaci par czas-warto��.
    std::vector<std::pair<wxString, double>> data;

    /// Deklaracja tabeli zdarze� dla tej klasy.
    DECLARE_EVENT_TABLE()
};

#endif