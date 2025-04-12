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
#include <fstream>
#include <wx/datetime.h>
#include <wx/filedlg.h>
#include <wx/icon.h>

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

    //Sizery do podzia³u apki na kilka podokien
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    label = new wxStaticText(panel, wxID_ANY, "Wybierz stacjê pomiarow¹:");

    // Lista rozwijana do wyboru stacji
    choice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(300, -1));
    choice->Bind(wxEVT_CHOICE, &MainFrame::WyswietlStanowiska, this);

    wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);

    // Filter
    cityInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));

    filterButton = new wxButton(panel, wxID_ANY, "Filtruj", wxDefaultPosition, wxSize(80, -1));
    filterButton->Bind(wxEVT_BUTTON, &MainFrame::FiltrujStacje, this);

    filterSizer->Add(cityInput, 0, wxALL, 5);
    filterSizer->Add(filterButton, 0, wxALL, 5);

    leftSizer->Add(label, 0, wxALL, 5);
    leftSizer->Add(choice, 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(filterSizer, 0, wxALL, 5);

    wxBoxSizer* dataButtonsSizer = new wxBoxSizer(wxHORIZONTAL);

    // Zapis i ³adowanie JSON
    saveButton = new wxButton(panel, wxID_ANY, "Zapisz dane", wxDefaultPosition, wxSize(120, -1));
    saveButton->Bind(wxEVT_BUTTON, &MainFrame::ZapiszDane, this);

    loadButton = new wxButton(panel, wxID_ANY, "Za³aduj dane", wxDefaultPosition, wxSize(120, -1));
    loadButton->Bind(wxEVT_BUTTON, &MainFrame::ZaladujDane, this);

    dataButtonsSizer->Add(saveButton, 0, wxALL, 5);
    dataButtonsSizer->Add(loadButton, 0, wxALL, 5);

    leftSizer->Add(dataButtonsSizer, 0, wxALL, 5);

    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    //Rodzaje zanieczyszczeñ
    infoLabel = new wxStaticText(panel, wxID_ANY, "Wybierz rodzaj zanieczyszczenia:");

    sensorList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(300, 100));

    sensorList->Bind(wxEVT_LISTBOX, &MainFrame::WyswietlDanePomiarowe, this);

    rightSizer->Add(infoLabel, 0, wxALL, 5);
    rightSizer->Add(sensorList, 1, wxALL | wxEXPAND, 5);

    topSizer->Add(leftSizer, 0, wxALL | wxEXPAND, 5);
    topSizer->Add(rightSizer, 1, wxALL | wxEXPAND, 5);

    resultBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(450, 80), wxTE_MULTILINE | wxTE_READONLY);

    // Wykres
    plotPanel = new PlotPanel(panel);

    wxBoxSizer* timeRangeSizer = new wxBoxSizer(wxHORIZONTAL);

    //Daty analizowanych danych
    wxStaticText* startDateLabel = new wxStaticText(panel, wxID_ANY, "Dolna granica:");
    wxStaticText* endDateLabel = new wxStaticText(panel, wxID_ANY, "Górnica granica:");

    startDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    endDateChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1));

    confirmButton = new wxButton(panel, wxID_ANY, "PotwierdŸ", wxDefaultPosition, wxSize(100, -1));
    confirmButton->Bind(wxEVT_BUTTON, &MainFrame::PotwierdzZakres, this);

    timeRangeSizer->Add(startDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(startDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(endDateLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    timeRangeSizer->Add(endDateChoice, 0, wxALL, 5);
    timeRangeSizer->Add(confirmButton, 0, wxALL, 5);

    mainSizer->Add(topSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(resultBox, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(plotPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(timeRangeSizer, 0, wxALL | wxALIGN_CENTER, 5);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    PobierzDane();

    // Ikona
    wxIcon myIcon;
    if (wxFileExists("cloud.ico")) {
        myIcon.LoadFile("cloud.ico", wxBITMAP_TYPE_ICO);
        SetIcon(myIcon);
    }
    else if (wxFileExists("./cloud.ico")) {
        myIcon.LoadFile("./cloud.ico", wxBITMAP_TYPE_ICO);
        SetIcon(myIcon);
    }
    else {
        wxLogWarning("Nie mo¿na znaleŸæ pliku ikony cloud.ico");
    }
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
        wxMessageBox("Nie uda³o siê pobraæ danych pomiarowych! SprawdŸ po³¹czenie internetowe lub za³aduj dane z pliku.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    if (!parseJsonResponse(response, currentSensorData)) {
        wxMessageBox("Brak danych pomiarowych lub b³¹d parsowania JSON!", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    AnalizujDane(currentSensorData);
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
        plotPanel->SetData({});
        return;
    }

    double minVal = DBL_MAX, maxVal = DBL_MIN, sum = 0;
    wxString minTime, maxTime;
    int count = 0;

    values.clear();

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

    plotPanel->SetData(values);

    timestamps.clear();
    startDateChoice->Clear();
    endDateChoice->Clear();

    for (const auto& entry : data["values"]) {
        if (entry["value"].isNull()) continue;

        wxString timestamp = wxString::FromUTF8(entry["date"].asString());
        timestamps.push_back(timestamp);
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

    wxString startTimestamp = startDateChoice->GetString(startIdx);
    wxString endTimestamp = endDateChoice->GetString(endIdx);

    if (startTimestamp > endTimestamp) {
        wxMessageBox("Dolna granica czasu nie mo¿e byæ póŸniejsza ni¿ górna granica.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    if (startTimestamp == endTimestamp) {
        wxMessageBox("Dolna granica czasu nie mo¿e byæ równa górnej granicy.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    std::vector<std::pair<wxString, double>> filteredValues;

    for (const auto& entry : values) {
        if (entry.first >= startTimestamp && entry.first <= endTimestamp) {
            filteredValues.push_back(entry);
        }
    }

    if (filteredValues.empty()) {
        wxMessageBox("Brak danych w wybranym zakresie czasu.", "B³¹d", wxOK | wxICON_ERROR);
        resultBox->SetValue("Brak danych w wybranym zakresie czasu.");
        plotPanel->SetData({});
        plotPanel->Refresh();
        return;
    }

    std::sort(filteredValues.begin(), filteredValues.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    plotPanel->SetData(filteredValues);
    plotPanel->Refresh();

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

    this->Update();
}

/**
 * @brief Zapisuje dane pomiarowe do pliku JSON.
 *
 * Ta metoda zapisuje aktualne dane pomiarowe do pliku JSON w katalogu aplikacji.
 * Nazwa pliku zawiera datê i czas pobrania danych.
 *
 * @param event Zdarzenie klikniêcia przycisku "Zapisz".
 */
void MainFrame::ZapiszDane(wxCommandEvent& event) {
    // Sprawdzamy, czy s¹ jakieœ dane do zapisania
    if (currentSensorData.empty() || !currentSensorData.isMember("values")) {
        wxMessageBox("Brak danych do zapisania. Najpierw wybierz stacjê i rodzaj zanieczyszczenia.",
            "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    wxString fileName = GenerujNazwePliku();

    std::ofstream file(fileName.ToStdString());
    if (!file.is_open()) {
        wxMessageBox("Nie mo¿na utworzyæ pliku " + fileName, "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value saveData;

    int stationIdx = choice->GetSelection();
    int sensorIdx = sensorList->GetSelection();

    if (stationIdx != wxNOT_FOUND && sensorIdx != wxNOT_FOUND) {
        wxString stationName = choice->GetString(stationIdx);
        wxString sensorName = sensorList->GetString(sensorIdx);

        saveData["stationName"] = stationName.ToStdString();
        saveData["sensorName"] = sensorName.ToStdString();
        saveData["data"] = currentSensorData;

        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    ";
        std::string jsonData = Json::writeString(writer, saveData);
        file << jsonData;
        file.close();

        wxMessageBox("Dane zosta³y zapisane do pliku:\n" + fileName, "Zapisano dane", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Nie wybrano stacji lub rodzaju zanieczyszczenia.", "B³¹d", wxOK | wxICON_ERROR);
    }
}

/**
 * @brief £aduje dane pomiarowe z pliku JSON.
 *
 * Ta metoda ³aduje historyczne dane pomiarowe z pliku JSON i wyœwietla je
 * w aplikacji.
 *
 * @param event Zdarzenie klikniêcia przycisku "Za³aduj".
 */
void MainFrame::ZaladujDane(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Wybierz plik z danymi", "", "", "Pliki JSON (*.json)|*.json",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ifstream file(openFileDialog.GetPath().ToStdString());
    if (!file.is_open()) {
        wxMessageBox("Nie mo¿na otworzyæ pliku " + openFileDialog.GetPath(), "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value loadedData;
    Json::CharReaderBuilder reader;
    std::string errors;

    bool parsingSuccessful = Json::parseFromStream(reader, file, &loadedData, &errors);
    file.close();

    if (!parsingSuccessful) {
        wxMessageBox("B³¹d podczas parsowania pliku JSON:\n" + wxString(errors), "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    if (!loadedData.isMember("stationName") || !loadedData.isMember("sensorName") || !loadedData.isMember("data")) {
        wxMessageBox("Nieprawid³owy format pliku. Brak wymaganych pól.", "B³¹d", wxOK | wxICON_ERROR);
        return;
    }

    wxString stationName = wxString::FromUTF8(loadedData["stationName"].asString());
    wxString sensorName = wxString::FromUTF8(loadedData["sensorName"].asString());

    bool stationFound = false;
    for (unsigned int i = 0; i < choice->GetCount(); i++) {
        if (choice->GetString(i) == stationName) {
            choice->SetSelection(i);
            stationFound = true;
            break;
        }
    }

    if (!stationFound && choice->GetCount() > 0) {
        choice->SetSelection(0);
    }

    currentSensorData = loadedData["data"];

    wxString infoText = "Za³adowano dane z pliku:\nStacja: " + stationName + "\nSensor: " + sensorName;
    wxMessageBox(infoText, "Dane za³adowane", wxOK | wxICON_INFORMATION);

    AnalizujDane(currentSensorData);
}

/**
 * @brief Generuje nazwê pliku JSON na podstawie aktualnej daty i czasu.
 *
 * @return wxString Nazwa pliku z rozszerzeniem JSON.
 */
wxString MainFrame::GenerujNazwePliku() {
    wxDateTime now = wxDateTime::Now();
    wxString fileName = now.Format("dane_pomiarowe_%Y%m%d_%H%M%S.json");
    return fileName;
}