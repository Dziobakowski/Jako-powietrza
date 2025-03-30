/**
 * @file MainFrame.cpp
 * @brief Implementacja g��wnego okna aplikacji.
 *
 * W tym pliku znajduj� si� funkcje obs�uguj�ce interakcj� z interfejsem u�ytkownika
 * oraz pobieranie, filtrowanie i analizowanie danych z API GIO�.
 *
 * @see MainFrame.h
 */

#include "MainFrame.h"
#include "PlotPanel.h"
#include "Curl.h"
#include <json/json.h>
#include <map>

 /**
  * @brief Konstruktor klasy MainFrame
  *
  * Tworzy g��wne okno aplikacji, inicjalizuje wszystkie komponenty interfejsu u�ytkownika
  * i ustawia je w odpowiednich sizerach. Pobiera r�wnie� dane ze stacji pomiarowych
  * z serwisu GIO� oraz aktualizuje list� stacji pomiarowych.
  *
  * @param title Tytu� okna aplikacji.
  */
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    wxPanel* panel = new wxPanel(this);

    // Tworzymy g��wny sizer o orientacji pionowej
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Tworzymy poziomy sizer dla cz�ci g�rnej interfejsu (wyb�r stacji i zanieczyszczenia)
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    // Tworzymy lewy pionowy sizer dla wyboru stacji
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    // Etykieta nad list� wyboru
    label = new wxStaticText(panel, wxID_ANY, "Wybierz stacj� pomiarow�:");

    // Lista rozwijana do wyboru stacji
    choice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(300, -1));
    choice->Bind(wxEVT_CHOICE, &MainFrame::WyswietlStanowiska, this); // Po wyborze stacji

    // Tworzymy sizer poziomy dla pola tekstowego i przycisku
    wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);

    // Pole tekstowe do wpisania miejscowo�ci
    cityInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));

    // Przycisk do filtrowania listy
    filterButton = new wxButton(panel, wxID_ANY, "Filtruj", wxDefaultPosition, wxSize(80, -1));
    filterButton->Bind(wxEVT_BUTTON, &MainFrame::FiltrujStacje, this);

    // Dodanie pola tekstowego i przycisku do poziomego sizeru
    filterSizer->Add(cityInput, 0, wxALL, 5);
    filterSizer->Add(filterButton, 0, wxALL, 5);

    // Dodajemy elementy wyboru stacji do lewego sizera
    leftSizer->Add(label, 0, wxALL, 5);
    leftSizer->Add(choice, 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(filterSizer, 0, wxALL, 5);

    // Tworzymy prawy pionowy sizer dla wyboru zanieczyszcze�
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Etykieta nad list� stanowisk pomiarowych
    infoLabel = new wxStaticText(panel, wxID_ANY, "Wybierz rodzaj zanieczyszczenia:");

    // Lista stanowisk pomiarowych
    sensorList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(300, 100));

    // Powi�zanie listy stanowisk z funkcj� pobierania danych
    sensorList->Bind(wxEVT_LISTBOX, &MainFrame::WyswietlDanePomiarowe, this);

    // Dodajemy elementy wyboru zanieczyszcze� do prawego sizera
    rightSizer->Add(infoLabel, 0, wxALL, 5);
    rightSizer->Add(sensorList, 1, wxALL | wxEXPAND, 5);

    // Dodajemy lewy i prawy sizer do g�rnego poziomego sizera
    topSizer->Add(leftSizer, 0, wxALL | wxEXPAND, 5);
    topSizer->Add(rightSizer, 1, wxALL | wxEXPAND, 5);

    // Pole tekstowe do wy�wietlania analizy danych
    resultBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(450, 80), wxTE_MULTILINE | wxTE_READONLY);

    // Tworzymy panel wykresu (PlotPanel)
    plotPanel = new PlotPanel(panel);  // Dodajemy panel wykresu

    // Tworzymy sizer poziomy dla dolnej cz�ci interfejsu (do wyboru zakresu czasowego)
    wxBoxSizer* timeRangeSizer = new wxBoxSizer(wxHORIZONTAL);

    // Etykiety dla rozwijanych list
    wxStaticText* startDateLabel = new wxStaticText(panel, wxID_ANY, "G�rna granica:");
    wxStaticText* endDateLabel = new wxStaticText(panel, wxID_ANY, "Dolna granica:");

    // Inicjalizacja list rozwijanych
    startDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    endDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));

    // Dodanie przycisku "Potwierd�"
    confirmButton = new wxButton(panel, wxID_ANY, "Potwierd�", wxDefaultPosition, wxSize(100, -1));
    confirmButton->Bind(wxEVT_BUTTON, &MainFrame::PotwierdzZakres, this);

    // Dodajemy elementy wyboru zakresu czasowego do sizera
    timeRangeSizer->Add(startDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(startDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(endDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(endDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(confirmButton, 0, wxALL, 5);

    // Dodajemy wszystkie g��wne elementy do g��wnego sizera
    mainSizer->Add(topSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(resultBox, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(plotPanel, 1, wxEXPAND | wxALL, 5); // Zwi�kszamy proporcj� dla wykresu
    mainSizer->Add(timeRangeSizer, 0, wxALL | wxALIGN_CENTER, 5);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this); // Dopasowuje rozmiar okna do zawarto�ci

    // Pobranie danych z API i aktualizacja listy
    PobierzDane();
}

/**
 * @brief Pobiera dane stacji pomiarowych z API GIO�.
 *
 * Funkcja wykonuje zapytanie do serwisu GIO� w celu pobrania listy dost�pnych stacji
 * pomiarowych, kt�re nast�pnie s� dodawane do rozwijanej listy stacji.
 * W przypadku b��du wy�wietlana jest odpowiednia informacja.
 *
 * @note W przypadku braku po��czenia z serwerem GIO� lub problemu z przetwarzaniem danych
 *       wy�wietlana jest wiadomo�� o b��dzie.
 */
void MainFrame::PobierzDane() {
    std::string response;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/station/findAll";

    if (!performCurlRequest(url, response)) {
        wxMessageBox("B��d: Brak po��czenia z serwerem GIO�. Sprawd� swoje po��czenie internetowe.", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root) || root.empty()) {
        wxMessageBox("B��d: Nie uda�o si� przetworzy� danych. Serwis GIO� mo�e by� chwilowo niedost�pny.", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    stationMap.clear();
    wxArrayString stationNames;

    try {
        for (int i = 0; i < root.size(); i++) {
            std::string stationName = root[i]["stationName"].asString();
            std::string cityName = root[i]["city"]["name"].asString();
            int stationId = root[i]["id"].asInt();

            wxString wxStationName = wxString::FromUTF8(stationName);
            wxString wxCityName = wxString::FromUTF8(cityName);

            stationMap[wxStationName + " (" + wxCityName + ")"] = stationId;
        }

        for (const auto& entry : stationMap) {
            stationNames.Add(entry.first);
        }

        choice->Append(stationNames);
    }
    catch (...) {
        wxMessageBox("Wyst�pi� nieoczekiwany b��d podczas przetwarzania danych.", "B��d", wxOK | wxICON_ERROR);
    }
}

/**
 * @brief Filtrowanie stacji na podstawie miejscowo�ci.
 *
 * Funkcja przeszukuje wprowadzone przez u�ytkownika miasto w celu filtrowania dost�pnych
 * stacji pomiarowych. List� stacji wy�wietla si� na podstawie wprowadzonego filtru.
 *
 * @param event Wydarzenie zwi�zane z przyciskiem "Filtruj".
 * @see PobierzDane
 */
void MainFrame::FiltrujStacje(wxCommandEvent& event) {
    wxString cityFilter = cityInput->GetValue().Lower();
    choice->Clear();

    wxArrayString filteredStations;
    for (const auto& entry : stationMap) {
        if (entry.first.Lower().Contains(cityFilter)) {
            filteredStations.Add(entry.first);
        }
    }

    if (filteredStations.IsEmpty()) {
        filteredStations.Add("Brak wynik�w");
    }

    choice->Append(filteredStations);
}

/**
 * @brief Wy�wietlanie stanowisk pomiarowych dla wybranej stacji.
 *
 * Funkcja pobiera dost�pne stanowiska pomiarowe dla wybranej stacji i wy�wietla je
 * w odpowiedniej li�cie. Pobieranie danych odbywa si� za pomoc� API GIO�.
 *
 * @param event Wydarzenie zwi�zane z wyborem stacji pomiarowej.
 * @see PobierzDane
 */
void MainFrame::WyswietlStanowiska(wxCommandEvent& event) {
    int selected = choice->GetSelection();
    if (selected == wxNOT_FOUND) return;

    wxString stationName = choice->GetString(selected);
    int stationId = stationMap[stationName];

    std::string url = "https://api.gios.gov.pl/pjp-api/rest/station/sensors/" + std::to_string(stationId);
    std::string response;

    if (!performCurlRequest(url, response)) {
        wxMessageBox("Nie uda�o si� pobra� stanowisk pomiarowych!", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root)) {
        wxMessageBox("Brak danych stanowisk!", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    sensorList->Clear();
    sensorMap.clear();  // Wyczy�� map� przed nowymi danymi

    for (const auto& sensor : root) {
        wxString paramName = wxString::FromUTF8(sensor["param"]["paramName"].asString());
        int sensorId = sensor["id"].asInt();

        sensorList->Append(paramName);
        sensorMap[paramName] = sensorId;  // Zapisz ID w mapie!
    }
}

/**
 * @brief Wy�wietlanie danych pomiarowych dla wybranego stanowiska.
 *
 * Funkcja pobiera dane pomiarowe dla wybranego stanowiska i analizuje je, wy�wietlaj�c
 * odpowiednie wyniki w polu tekstowym oraz na wykresie.
 *
 * @param event Wydarzenie zwi�zane z wyborem stanowiska pomiarowego.
 * @see WyswietlStanowiska
 */
void MainFrame::WyswietlDanePomiarowe(wxCommandEvent& event) {
    int selected = sensorList->GetSelection();
    if (selected == wxNOT_FOUND) return;

    wxString sensorName = sensorList->GetString(selected);
    int sensorId = sensorMap[sensorName];

    std::string url = "https://api.gios.gov.pl/pjp-api/rest/data/getData/" + std::to_string(sensorId);
    std::string response;

    if (!performCurlRequest(url, response)) {
        wxMessageBox("Nie uda�o si� pobra� danych pomiarowych!", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root)) {
        wxMessageBox("Brak danych pomiarowych!", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    AnalizujDane(root);
}

/**
 * @brief Analizuje dane pomiarowe.
 *
 * Funkcja analizuje dane pomiarowe, wylicza warto�ci minimalne, maksymalne, �rednie
 * oraz okre�la trend (wzrostowy/malej�cy). Wyniki s� wy�wietlane w polu tekstowym oraz na wykresie.
 *
 * @param data Dane pomiarowe w formacie JSON.
 * @see WyswietlDanePomiarowe
 */
void MainFrame::AnalizujDane(const Json::Value& data) {
    if (data["values"].empty()) {
        resultBox->SetValue("Brak danych pomiarowych.");
        plotPanel->SetData({}); // Czy�ci wykres
        return;
    }

    double minVal = DBL_MAX, maxVal = DBL_MIN, sum = 0;
    wxString minTime, maxTime;
    int count = 0;

    values.clear(); // Wyczy�� poprzednie dane

    for (const auto& entry : data["values"]) {
        if (entry["value"].isNull()) continue;

        double value = entry["value"].asDouble();
        wxString timestamp = wxString::FromUTF8(entry["date"].asString());

        values.push_back({ timestamp, value });
        sum += value;
        count++;

        if (value < minVal) {
            minVal = value;
            minTime = timestamp;
        }
        if (value > maxVal) {
            maxVal = value;
            maxTime = timestamp;
        }
    }

    double avgVal = count > 0 ? sum / count : 0.0;
    wxString trend = (values.size() > 1 && values.front().second < values.back().second) ? "Wzrostowy" : "Malej�cy";

    wxString result;
    result << "Analiza danych:\n"
        << "Min: " << minVal << " (" << minTime << ")\n"
        << "Max: " << maxVal << " (" << maxTime << ")\n"
        << "�rednia: " << avgVal << "\n"
        << "Trend: " << trend;

    resultBox->SetValue(result);

    plotPanel->SetData(values); // Aktualizacja wykresu

    timestamps.clear();  // Wyczy�� poprzednie dane
    startDateChoice->Clear();
    endDateChoice->Clear();

    for (const auto& entry : data["values"]) {
        if (entry["value"].isNull()) continue;

        wxString timestamp = wxString::FromUTF8(entry["date"].asString());
        timestamps.push_back(timestamp);  // Dodaj do listy dost�pnych znacznik�w czasu
    }

    for (const auto& timestamp : timestamps) {
        startDateChoice->Append(timestamp);
        endDateChoice->Append(timestamp);
    }
}
/**
 * @brief Obs�uguje zdarzenie klikni�cia przycisku "Potwierd�" i filtruje dane pomiarowe na podstawie wybranego zakresu czasowego.
 *
 * Funkcja pobiera wybrane indeksy dat z kontrolek startDateChoice i endDateChoice, a nast�pnie filtruje dane pomiarowe,
 * uwzgl�dniaj�c jedynie te pr�bki, kt�rych znaczniki czasowe mieszcz� si� w okre�lonym przedziale. Je�eli nie zostanie wybrany
 * prawid�owy zakres lub brak jest danych w danym zakresie, u�ytkownik otrzymuje odpowiedni komunikat.
 *
 * @param event Referencja do obiektu wxCommandEvent reprezentuj�cego zdarzenie klikni�cia przycisku.
 *
 * @remarks Funkcja aktualizuje dane wy�wietlane na wykresie za pomoc� plotPanel->SetData().
 *
 * @see MainFrame::startDateChoice
 * @see MainFrame::endDateChoice
 * @see MainFrame::plotPanel
 */
void MainFrame::PotwierdzZakres(wxCommandEvent& event) {
    int startIdx = startDateChoice->GetSelection();
    int endIdx = endDateChoice->GetSelection();

    if (startIdx == wxNOT_FOUND || endIdx == wxNOT_FOUND) {
        wxMessageBox("Prosz� wybra� zakres czasowy.", "B��d", wxOK | wxICON_ERROR);
        return;
    }

    // Pami�taj, �e dla danych czasowych cz�sto wcze�niejsza data (g�rna granica)
    // ma ni�szy indeks w tablicy ni� p�niejsza data (dolna granica)
    if (startIdx > endIdx) {
        // Mo�emy zamieni� indeksy, je�li u�ytkownik wybra� je w odwrotnej kolejno�ci
        std::swap(startIdx, endIdx);
    }

    wxString startTimestamp = timestamps[startIdx];
    wxString endTimestamp = timestamps[endIdx];

    std::vector<std::pair<wxString, double>> filteredValues;

    for (const auto& entry : values) {
        // Sprawd� czy warto�� mie�ci si� w zakresie czasowym
        // Uwaga: format daty mo�e wymaga� odpowiedniego por�wnania
        if (entry.first >= startTimestamp && entry.first <= endTimestamp) {
            filteredValues.push_back(entry);
        }
    }

    if (filteredValues.empty()) {
        resultBox->SetValue("Brak danych w wybranym zakresie czasu.");
        plotPanel->SetData({});
        return;
    }

    // Zaktualizuj wy�wietlane dane
    plotPanel->SetData(filteredValues);

    // Aktualizacja informacji o zakresie w resultBox
    double minVal = DBL_MAX, maxVal = DBL_MIN, sum = 0;
    wxString minTime, maxTime;
    int count = 0;

    for (const auto& entry : filteredValues) {
        double value = entry.second;
        sum += value;
        count++;

        if (value < minVal) {
            minVal = value;
            minTime = entry.first;
        }
        if (value > maxVal) {
            maxVal = value;
            maxTime = entry.first;
        }
    }

    double avgVal = count > 0 ? sum / count : 0.0;
    wxString trend = (filteredValues.size() > 1 && filteredValues.front().second < filteredValues.back().second) ? "Wzrostowy" : "Malej�cy";

    wxString result;
    result << "Analiza danych w zakresie " << startTimestamp << " - " << endTimestamp << ":\n"
        << "Min: " << minVal << " (" << minTime << ")\n"
        << "Max: " << maxVal << " (" << maxTime << ")\n"
        << "�rednia: " << avgVal << "\n"
        << "Trend: " << trend;

    resultBox->SetValue(result);
}