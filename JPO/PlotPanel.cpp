#include "PlotPanel.h"

BEGIN_EVENT_TABLE(PlotPanel, wxPanel)
EVT_PAINT(PlotPanel::OnPaint) ///< Rejestruje zdarzenie malowania, wywo³uje metodê `OnPaint`.
END_EVENT_TABLE()

/**
 * @brief Konstruktor klasy `PlotPanel`.
 *
 * Inicjalizuje panel wykresu w aplikacji, ustawiaj¹c jego wymiary i pozycjê.
 *
 * @param parent Okno, w którym panel bêdzie osadzony.
 */
    PlotPanel::PlotPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 300)) {
}

/**
 * @brief Ustawia dane wykresu.
 *
 * Metoda ta ustawia dane wykresu na podstawie podanych par (czas, wartoœæ) oraz
 * odœwie¿a panel, aby wyœwietliæ nowy wykres.
 *
 * @param newData Wektor par (czas, wartoœæ), które bêd¹ u¿yte do narysowania wykresu.
 */
void PlotPanel::SetData(const std::vector<std::pair<wxString, double>>& newData) {
    data = newData;
    Refresh(); ///< Odœwie¿enie panelu, aby zaktualizowaæ wykres.
}

/**
 * @brief Funkcja maluj¹ca wykres.
 *
 * Funkcja ta jest wywo³ywana przy ka¿dym zdarzeniu malowania. Rysuje wykres na
 * panelu na podstawie danych dostarczonych przez metodê `SetData`.
 * Jeœli brak jest danych, wyœwietla komunikat "Brak danych".
 *
 * @param event Zdarzenie malowania, dostarczane przez wxWidgets. U¿ywane do
 *              tworzenia obiektu `wxPaintDC` do malowania na panelu.
 */
void PlotPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this); ///< Obiekt odpowiedzialny za malowanie na panelu.
    dc.SetBrush(*wxWHITE_BRUSH); ///< Ustawia kolor t³a na bia³y.
    dc.Clear(); ///< Wyczyœæ panel przed narysowaniem nowego wykresu.

    if (data.empty()) {
        dc.DrawText("Brak danych", 10, 10); ///< Wyœwietla komunikat, gdy brak danych.
        return;
    }

    int width, height;
    GetClientSize(&width, &height); ///< Pobiera wymiary panelu.

    int margin = 60; ///< Zwiêkszone marginesy dla wiêkszej iloœci miejsca na etykiety osi
    int graphWidth = width - 2 * margin; ///< Szerokoœæ wykresu.
    int graphHeight = height - 2 * margin; ///< Wysokoœæ wykresu.

    double minValue = DBL_MAX, maxValue = DBL_MIN;
    for (const auto& entry : data) {
        if (entry.second < minValue) minValue = entry.second;
        if (entry.second > maxValue) maxValue = entry.second;
    }

    minValue = floor(minValue * 0.95);
    maxValue = ceil(maxValue * 1.05);

    // Oœ czasu
    int numPoints = data.size();
    double stepX = static_cast<double>(graphWidth) / (numPoints - 1);

    // Oœ wartoœci
    double range = maxValue - minValue;
    if (range == 0) range = 1; ///< Zapobiega dzieleniu przez 0, jeœli wszystkie wartoœci s¹ równe.

    dc.SetPen(*wxBLACK_PEN); ///< Ustawia kolor rysowania osi na czarny.
    dc.DrawLine(margin, height - margin, width - margin, height - margin); ///< Oœ X.
    dc.DrawLine(margin, margin, margin, height - margin); ///< Oœ Y.

    const int numYTicks = 5;
    for (int i = 0; i <= numYTicks; ++i) {
        double value = minValue + (i * range / numYTicks);
        int y = height - margin - (i * graphHeight / numYTicks);

        dc.DrawLine(margin - 5, y, margin, y);

        wxString label = wxString::Format("%.1f", value);
        wxSize textSize = dc.GetTextExtent(label);
        dc.DrawText(label, margin - textSize.GetWidth() - 8, y - textSize.GetHeight() / 2);
    }

    const int numXTicks = std::min(5, numPoints);
    if (numPoints > 1) {
        for (int i = 0; i < numXTicks; ++i) {
            int dataIndex = i * (numPoints - 1) / (numXTicks - 1);
            if (dataIndex >= numPoints) dataIndex = numPoints - 1;

            int x = margin + dataIndex * stepX;

            dc.DrawLine(x, height - margin, x, height - margin + 5);

            wxString timestamp = data[dataIndex].first;
            if (timestamp.Length() > 5) {
                timestamp = timestamp.Left(16);
            }

            wxSize textSize = dc.GetTextExtent(timestamp);
            dc.DrawText(timestamp, x - textSize.GetWidth() / 2, height - margin + 8);
        }
    }

    dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxString yAxisTitle = "Wartoœæ";
    wxSize yTitleSize = dc.GetTextExtent(yAxisTitle);
    dc.DrawRotatedText(yAxisTitle, margin - 45, height / 2 - yTitleSize.GetWidth() / 2, 90);

    wxString xAxisTitle = "Czas";
    wxSize xTitleSize = dc.GetTextExtent(xAxisTitle);
    dc.DrawText(xAxisTitle, width / 2 - xTitleSize.GetWidth() / 2, height - margin + 40);

    dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    dc.SetPen(*wxBLUE_PEN); ///< Ustawia kolor rysowania punktów na niebieski.
    for (size_t i = 0; i < data.size(); i++) {
        int x = margin + i * stepX; ///< Oblicza wspó³rzêdn¹ X punktu.
        int y = height - margin - ((data[i].second - minValue) / range) * graphHeight; ///< Oblicza wspó³rzêdn¹ Y punktu.

        dc.DrawCircle(x, y, 3); ///< Rysuje punkt na wykresie.
        if (i > 0) {
            int prevX = margin + (i - 1) * stepX; ///< Oblicza wspó³rzêdn¹ X poprzedniego punktu.
            int prevY = height - margin - ((data[i - 1].second - minValue) / range) * graphHeight; ///< Oblicza wspó³rzêdn¹ Y poprzedniego punktu.
            dc.DrawLine(prevX, prevY, x, y); ///< £¹czy punkt z poprzednim punktem na wykresie.
        }
    }
}