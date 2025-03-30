#ifndef PLOT_PANEL_H
#define PLOT_PANEL_H

#include <wx/wx.h>
#include <vector>

/**
 * @file PlotPanel.h
 * @brief Nag³ówek panelu do wyœwietlania wykresu.
 *
 * Plik zawiera deklaracjê klasy `PlotPanel`, która dziedziczy po klasie `wxPanel`
 * i jest odpowiedzialna za wyœwietlanie wykresu na panelu w aplikacji wxWidgets.
 */

 /**
  * @class PlotPanel
  * @brief Klasa reprezentuj¹ca panel wykresu.
  *
  * Klasa `PlotPanel` dziedziczy po klasie `wxPanel` i odpowiada za rysowanie wykresu
  * na panelu. Zawiera metody umo¿liwiaj¹ce ustawianie danych wykresu oraz jego rysowanie
  * w odpowiedzi na zdarzenia zwi¹zane z malowaniem.
  */
class PlotPanel : public wxPanel {
public:
    /**
     * @brief Konstruktor klasy `PlotPanel`.
     *
     * Tworzy panel wykresu w aplikacji. Inicjalizuje komponenty panelu, w tym
     * rejestruje zdarzenia rysowania wykresu.
     *
     * @param parent Okno, w którym panel bêdzie osadzony.
     */
    PlotPanel(wxWindow* parent);

    /**
     * @brief Ustawia dane wykresu.
     *
     * Funkcja ta ustawia dane wykresu na podstawie podanych par czas-wartoœæ.
     * Dane s¹ przechowywane w wektorze i u¿ywane do rysowania wykresu.
     *
     * @param newData Dane wykresu w postaci wektora par (czas, wartoœæ).
     */
    void SetData(const std::vector<std::pair<wxString, double>>& newData);

    /**
     * @brief Funkcja maluj¹ca wykres.
     *
     * Funkcja ta jest wywo³ywana przy ka¿dym zdarzeniu malowania. Odpowiada
     * za renderowanie wykresu na panelu na podstawie danych dostarczonych przez
     * metodê `SetData`.
     *
     * @param event Zdarzenie malowania (dostarczane przez wxWidgets).
     */
    void OnPaint(wxPaintEvent& event);

private:
    /// Przechowuje dane wykresu w postaci par czas-wartoœæ.
    std::vector<std::pair<wxString, double>> data;

    /// Deklaracja tabeli zdarzeñ dla tej klasy.
    DECLARE_EVENT_TABLE()
};

#endif