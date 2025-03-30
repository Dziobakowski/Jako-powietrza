/**
 * @file MainFrame.cpp
 * @brief Implementacja g³ównego okna aplikacji.
 *
 * W tym pliku znajduj¹ siê funkcje obs³uguj¹ce interakcjê z interfejsem u¿ytkownika
 * oraz pobieranie, filtrowanie i analizowanie danych z API GIOŒ.
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
  * Tworzy g³ówne okno aplikacji, inicjalizuje wszystkie komponenty interfejsu u¿ytkownika
  * i ustawia je w odpowiednich sizerach. Pobiera równie¿ dane ze stacji pomiarowych
  * z serwisu GIOŒ oraz aktualizuje listê stacji pomiarowych.
  *
  * @param title Tytu³ okna aplikacji.
  */
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    wxPanel* panel = new wxPanel(this);

    // Tworzymy g³ówny sizer o orientacji pionowej
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Tworzymy poziomy sizer dla czêœci górnej interfejsu (wybór stacji i zanieczyszczenia)
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    // Tworzymy lewy pionowy sizer dla wyboru stacji
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    // Etykieta nad list¹ wyboru
    label = new wxStaticText(panel, wxID_ANY, "Wybierz stacjê pomiarow¹:");

    // Lista rozwijana do wyboru stacji
    choice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(300, -1));
    choice->Bind(wxEVT_CHOICE, &MainFrame::WyswietlStanowiska, this); // Po wyborze stacji

    // Tworzymy sizer poziomy dla pola tekstowego i przycisku
    wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);

    // Pole tekstowe do wpisania miejscowoœci
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

    // Tworzymy prawy pionowy sizer dla wyboru zanieczyszczeñ
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Etykieta nad list¹ stanowisk pomiarowych
    infoLabel = new wxStaticText(panel, wxID_ANY, "Wybierz rodzaj zanieczyszczenia:");

    // Lista stanowisk pomiarowych
    sensorList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(300, 100));

    // Powi¹zanie listy stanowisk z funkcj¹ pobierania danych
    sensorList->Bind(wxEVT_LISTBOX, &MainFrame::WyswietlDanePomiarowe, this);

    // Dodajemy elementy wyboru zanieczyszczeñ do prawego sizera
    rightSizer->Add(infoLabel, 0, wxALL, 5);
    rightSizer->Add(sensorList, 1, wxALL | wxEXPAND, 5);

    // Dodajemy lewy i prawy sizer do górnego poziomego sizera
    topSizer->Add(leftSizer, 0, wxALL | wxEXPAND, 5);
    topSizer->Add(rightSizer, 1, wxALL | wxEXPAND, 5);

    // Pole tekstowe do wyœwietlania analizy danych
    resultBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(450, 80), wxTE_MULTILINE | wxTE_READONLY);

    // Tworzymy panel wykresu (PlotPanel)
    plotPanel = new PlotPanel(panel);  // Dodajemy panel wykresu

    // Tworzymy sizer poziomy dla dolnej czêœci interfejsu (do wyboru zakresu czasowego)
    wxBoxSizer* timeRangeSizer = new wxBoxSizer(wxHORIZONTAL);

    // Etykiety dla rozwijanych list
    wxStaticText* startDateLabel = new wxStaticText(panel, wxID_ANY, "Górna granica:");
    wxStaticText* endDateLabel = new wxStaticText(panel, wxID_ANY, "Dolna granica:");

    // Inicjalizacja list rozwijanych
    startDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    endDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));

    // Dodanie przycisku "PotwierdŸ"
    confirmButton = new wxButton(panel, wxID_ANY, "PotwierdŸ", wxDefaultPosition, wxSize(100, -1));
    confirmButton->Bind(wxEVT_BUTTON, &MainFrame::PotwierdzZakres, this);

    // Dodajemy elementy wyboru zakresu czasowego do sizera
    timeRangeSizer->Add(startDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(startDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(endDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(endDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(confirmButton, 0, wxALL, 5);

    // Dodajemy wszystkie g³ówne elementy do g³ównego sizera
    mainSizer->Add(topSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(resultBox, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(plotPanel, 1, wxEXPAND | wxALL, 5); // Zwiêkszamy proporcjê dla wykresu
    mainSizer->Add(timeRangeSizer, 0, wxALL | wxALIGN_CENTER, 5);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this); // Dopasowuje rozmiar okna do zawartoœci

    // Pobranie danych z API i aktualizacja listy
    PobierzDane();
}

/**
 * @brief Pobiera dane stacji pomiarowych z API GIOŒ.
 *
 * Funkcja wykonuje zapytanie do serwisu GIOŒ w celu pobrania listy dostêpnych stacji
 * pomiarowych, które nastêpnie s¹ dodawane do rozwijanej listy stacji.
 * W przypadku b³êdu wyœwietlana jest odpowiednia informacja.
 *
 * @note W przypadku braku po³¹czenia z serwerem GIOŒ lub problemu z przetwarzaniem danych
 *       wyœwietlana jest wiadomoœæ o b³êdzie.
 */
void MainFrame::PobierzDane() {
    std::string response;
    std::string url = "https://api.gios.gov.pl/pjp-api/rest/station/findAll";

    if (!performCurlRequest(url, response)) {
        wxMessageBox("B³¹d: Brak po³¹czenia z serwerem GIOŒ. SprawdŸ swoje po³¹czenie internetowe.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root) || root.empty()) {
        wxMessageBox("B³¹d: Nie uda³o siê przetworzyæ danych. Serwis GIOŒ mo¿e byæ chwilowo niedostêpny.", "B³¹d", wxOK | wxICON_ERROR);
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
        wxMessageBox("Wyst¹pi³ nieoczekiwany b³¹d podczas przetwarzania danych.", "B³¹d", wxOK | wxICON_ERROR);
    }
}

/**
 * @brief Filtrowanie stacji na podstawie miejscowoœci.
 *
 * Funkcja przeszukuje wprowadzone przez u¿ytkownika miasto w celu filtrowania dostêpnych
 * stacji pomiarowych. Listê stacji wyœwietla siê na podstawie wprowadzonego filtru.
 *
 * @param event Wydarzenie zwi¹zane z przyciskiem "Filtruj".
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
        filteredStations.Add("Brak wyników");
    }

    choice->Append(filteredStations);
}

/**
 * @brief Wyœwietlanie stanowisk pomiarowych dla wybranej stacji.
 *
 * Funkcja pobiera dostêpne stanowiska pomiarowe dla wybranej stacji i wyœwietla je
 * w odpowiedniej liœcie. Pobieranie danych odbywa siê za pomoc¹ API GIOŒ.
 *
 * @param event Wydarzenie zwi¹zane z wyborem stacji pomiarowej.
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
        wxMessageBox("Nie uda³o siê pobraæ stanowisk pomiarowych!", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root)) {
        wxMessageBox("Brak danych stanowisk!", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    sensorList->Clear();
    sensorMap.clear();  // Wyczyœæ mapê przed nowymi danymi

    for (const auto& sensor : root) {
        wxString paramName = wxString::FromUTF8(sensor["param"]["paramName"].asString());
        int sensorId = sensor["id"].asInt();

        sensorList->Append(paramName);
        sensorMap[paramName] = sensorId;  // Zapisz ID w mapie!
    }
}

/**
 * @brief Wyœwietlanie danych pomiarowych dla wybranego stanowiska.
 *
 * Funkcja pobiera dane pomiarowe dla wybranego stanowiska i analizuje je, wyœwietlaj¹c
 * odpowiednie wyniki w polu tekstowym oraz na wykresie.
 *
 * @param event Wydarzenie zwi¹zane z wyborem stanowiska pomiarowego.
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
        wxMessageBox("Nie uda³o siê pobraæ danych pomiarowych!", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value root;
    if (!parseJsonResponse(response, root)) {
        wxMessageBox("Brak danych pomiarowych!", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    AnalizujDane(root);
}

/**
 * @brief Analizuje dane pomiarowe.
 *
 * Funkcja analizuje dane pomiarowe, wylicza wartoœci minimalne, maksymalne, œrednie
 * oraz okreœla trend (wzrostowy/malej¹cy). Wyniki s¹ wyœwietlane w polu tekstowym oraz na wykresie.
 *
 * @param data Dane pomiarowe w formacie JSON.
 * @see WyswietlDanePomiarowe
 */
void MainFrame::AnalizujDane(const Json::Value& data) {
    if (data["values"].empty()) {
        resultBox->SetValue("Brak danych pomiarowych.");
        plotPanel->SetData({}); // Czyœci wykres
        return;
    }

    double minVal = DBL_MAX, maxVal = DBL_MIN, sum = 0;
    wxString minTime, maxTime;
    int count = 0;

    values.clear(); // Wyczyœæ poprzednie dane

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
    wxString trend = (values.size() > 1 && values.front().second < values.back().second) ? "Wzrostowy" : "Malej¹cy";

    wxString result;
    result << "Analiza danych:\n"
        << "Min: " << minVal << " (" << minTime << ")\n"
        << "Max: " << maxVal << " (" << maxTime << ")\n"
        << "Œrednia: " << avgVal << "\n"
        << "Trend: " << trend;

    resultBox->SetValue(result);

    plotPanel->SetData(values); // Aktualizacja wykresu

    timestamps.clear();  // Wyczyœæ poprzednie dane
    startDateChoice->Clear();
    endDateChoice->Clear();

    for (const auto& entry : data["values"]) {
        if (entry["value"].isNull()) continue;

        wxString timestamp = wxString::FromUTF8(entry["date"].asString());
        timestamps.push_back(timestamp);  // Dodaj do listy dostêpnych znaczników czasu
    }

    for (const auto& timestamp : timestamps) {
        startDateChoice->Append(timestamp);
        endDateChoice->Append(timestamp);
    }
}
/**
 * @brief Obs³uguje zdarzenie klikniêcia przycisku "PotwierdŸ" i filtruje dane pomiarowe na podstawie wybranego zakresu czasowego.
 *
 * Funkcja pobiera wybrane indeksy dat z kontrolek startDateChoice i endDateChoice, a nastêpnie filtruje dane pomiarowe,
 * uwzglêdniaj¹c jedynie te próbki, których znaczniki czasowe mieszcz¹ siê w okreœlonym przedziale. Je¿eli nie zostanie wybrany
 * prawid³owy zakres lub brak jest danych w danym zakresie, u¿ytkownik otrzymuje odpowiedni komunikat.
 *
 * @param event Referencja do obiektu wxCommandEvent reprezentuj¹cego zdarzenie klikniêcia przycisku.
 *
 * @remarks Funkcja aktualizuje dane wyœwietlane na wykresie za pomoc¹ plotPanel->SetData().
 *
 * @see MainFrame::startDateChoice
 * @see MainFrame::endDateChoice
 * @see MainFrame::plotPanel
 */
void MainFrame::PotwierdzZakres(wxCommandEvent& event) {
    int startIdx = startDateChoice->GetSelection();
    int endIdx = endDateChoice->GetSelection();

    if (startIdx == wxNOT_FOUND || endIdx == wxNOT_FOUND) {
        wxMessageBox("Proszê wybraæ zakres czasowy.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    // Pamiêtaj, ¿e dla danych czasowych czêsto wczeœniejsza data (górna granica)
    // ma ni¿szy indeks w tablicy ni¿ póŸniejsza data (dolna granica)
    if (startIdx > endIdx) {
        // Mo¿emy zamieniæ indeksy, jeœli u¿ytkownik wybra³ je w odwrotnej kolejnoœci
        std::swap(startIdx, endIdx);
    }

    wxString startTimestamp = timestamps[startIdx];
    wxString endTimestamp = timestamps[endIdx];

    std::vector<std::pair<wxString, double>> filteredValues;

    for (const auto& entry : values) {
        // SprawdŸ czy wartoœæ mieœci siê w zakresie czasowym
        // Uwaga: format daty mo¿e wymagaæ odpowiedniego porównania
        if (entry.first >= startTimestamp && entry.first <= endTimestamp) {
            filteredValues.push_back(entry);
        }
    }

    if (filteredValues.empty()) {
        resultBox->SetValue("Brak danych w wybranym zakresie czasu.");
        plotPanel->SetData({});
        return;
    }

    // Zaktualizuj wyœwietlane dane
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
    wxString trend = (filteredValues.size() > 1 && filteredValues.front().second < filteredValues.back().second) ? "Wzrostowy" : "Malej¹cy";

    wxString result;
    result << "Analiza danych w zakresie " << startTimestamp << " - " << endTimestamp << ":\n"
        << "Min: " << minVal << " (" << minTime << ")\n"
        << "Max: " << maxVal << " (" << maxTime << ")\n"
        << "Œrednia: " << avgVal << "\n"
        << "Trend: " << trend;

    resultBox->SetValue(result);
}