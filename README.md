# Jakość-powietrza-w-Polsce
Projekt: Aplikacja do analizy danych o jakości powietrza

Opis:
Aplikacja służy do analizy danych o jakości powietrza, pobieranych z API Głównego Inspektoratu Ochrony Środowiska (GIOŚ).
Umożliwia filtrowanie stacji pomiarowych po nazwie miasta, wyświetlanie danych pomiarowych, analizowanie wyników (min, max, średnia, trend) oraz generowanie wykresów w zadanych okresach czasu.

Wymagania do edycji:
- wxWidgets (do tworzenia GUI)
- C++11 lub wyższy
- Biblioteka cURL (do pobierania danych z API)
- Kompilator obsługujący C++11 lub nowszy (np. GCC, MSVC)

Wykorzystanie:
1. Pobierz projekt z repozytorium GitHub:
   https://github.com/Dziobakowski/Jako-powietrza
2. Zainstaluj wymagane biblioteki:
   - wxWidgets (instrukcja na https://www.wxwidgets.org)
   - cURL (instrukcja na https://curl.se/libcurl/)
3. Skompiluj projekt, używając wybranego kompilatora:
   - Dla GCC: g++ -o air_quality_analysis main.cpp -lwxgtk3.0 -lcurl
4. Uruchom aplikację:
   - Dla systemów Linux: ./air_quality_analysis
   - Dla systemów Windows: air_quality_analysis.exe

Funkcje:
- Pobieranie danych o jakości powietrza z API GIOŚ.
- Filtrowanie stacji pomiarowych po nazwie miasta.
- Wyświetlanie danych pomiarowych w postaci tekstowej oraz na wykresie.
- Analiza danych (minimum, maksimum, średnia, trend).
- Możliwość filtrowania danych według zakresu czasowego.

W razie problemów lub pytań, skontaktuj się z autorem projektu (proszę, nie ~Autor).

Autor:
Tymon Wolniewicz

Licencja:
MIT License
