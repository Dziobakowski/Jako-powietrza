#include "PlotPanel.h"

BEGIN_EVENT_TABLE(PlotPanel, wxPanel)
EVT_PAINT(PlotPanel::OnPaint) ///< Rejestruje zdarzenie malowania, wywołuje metodę `OnPaint`.
END_EVENT_TABLE()

/**
 * @brief Konstruktor klasy `PlotPanel`.
 *
 * Inicjalizuje panel wykresu w aplikacji, ustawiając jego wymiary i pozycję.
 *
 * @param parent Okno, w którym panel będzie osadzony.
 */
    PlotPanel::PlotPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 300)) {
}

/**
 * @brief Ustawia dane wykresu.
 *
 * Metoda ta ustawia dane wykresu na podstawie podanych par (czas, wartość) oraz
 * odświeża panel, aby wyświetlić nowy wykres.
 *
 * @param newData Wektor par (czas, wartość), które będą użyte do narysowania wykresu.
 */
void PlotPanel::SetData(const std::vector<std::pair<wxString, double>>& newData) {
    data = newData;
    Refresh(); ///< Odświeżenie panelu, aby zaktualizować wykres.
}

/**
 * @brief Funkcja malująca wykres.
 *
 * Funkcja ta jest wywoływana przy każdym zdarzeniu malowania. Rysuje wykres na
 * panelu na podstawie danych dostarczonych przez metodę `SetData`.
 * Jeśli brak jest danych, wyświetla komunikat "Brak danych".
 *
 * @param event Zdarzenie malowania, dostarczane przez wxWidgets. Używane do
 *              tworzenia obiektu `wxPaintDC` do malowania na panelu.
 */
void PlotPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this); ///< Obiekt odpowiedzialny za malowanie na panelu.
    dc.SetBrush(*wxWHITE_BRUSH); ///< Ustawia kolor tła na biały.
    dc.Clear(); ///< Wyczyść panel przed narysowaniem nowego wykresu.

    if (data.empty()) {
        dc.DrawText("Brak danych", 10, 10); ///< Wyświetla komunikat, gdy brak danych.
        return;
    }

    int width, height;
    GetClientSize(&width, &height); ///< Pobiera wymiary panelu.

    int margin = 40; ///< Marginesy wokół wykresu.
    int graphWidth = width - 2 * margin; ///< Szerokość wykresu.
    int graphHeight = height - 2 * margin; ///< Wysokość wykresu.

    // Znalezienie min i max wartości
    double minValue = DBL_MAX, maxValue = DBL_MIN;
    for (const auto& entry : data) {
        if (entry.second < minValue) minValue = entry.second;
        if (entry.second > maxValue) maxValue = entry.second;
    }

    // Oś X (czas)
    int numPoints = data.size();
    double stepX = static_cast<double>(graphWidth) / (numPoints - 1);

    // Oś Y (wartości)
    double range = maxValue - minValue;
    if (range == 0) range = 1; ///< Zapobiega dzieleniu przez 0, jeśli wszystkie wartości są równe.

    // Rysowanie osi
    dc.SetPen(*wxBLACK_PEN); ///< Ustawia kolor rysowania osi na czarny.
    dc.DrawLine(margin, height - margin, width - margin, height - margin); ///< Oś X.
    dc.DrawLine(margin, margin, margin, height - margin); ///< Oś Y.

    // Rysowanie punktów na wykresie
    dc.SetPen(*wxBLUE_PEN); ///< Ustawia kolor rysowania punktów na niebieski.
    for (size_t i = 0; i < data.size(); i++) {
        int x = margin + i * stepX; ///< Oblicza współrzędną X punktu.
        int y = height - margin - ((data[i].second - minValue) / range) * graphHeight; ///< Oblicza współrzędną Y punktu.

        dc.DrawCircle(x, y, 3); ///< Rysuje punkt na wykresie.
        if (i > 0) {
            int prevX = margin + (i - 1) * stepX; ///< Oblicza współrzędną X poprzedniego punktu.
            int prevY = height - margin - ((data[i - 1].second - minValue) / range) * graphHeight; ///< Oblicza współrzędną Y poprzedniego punktu.
            dc.DrawLine(prevX, prevY, x, y); ///< Łączy punkt z poprzednim punktem na wykresie.
        }
    }
}