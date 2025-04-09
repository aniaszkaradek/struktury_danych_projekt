#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

// Mierzy czas wykonania funkcji
template <typename Funkcja>
long long zmierz_czas(Funkcja f) {
    auto start = high_resolution_clock::now();
    f();
    auto koniec = high_resolution_clock::now();
    return duration_cast<milliseconds>(koniec - start).count();
}

// Dodaj na początek tablicy dynamicznej
void dodaj_do_tablicy_na_poczatek(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc) {
    if (rozmiar == pojemnosc) {
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < rozmiar; ++i)
            nowa_tablica[i + 1] = tablica[i];
        nowa_tablica[0] = wartosc;
        delete[] tablica;
        tablica = nowa_tablica;
        ++rozmiar;
    }
    else {
        for (int i = rozmiar; i > 0; --i)
            tablica[i] = tablica[i - 1];
        tablica[0] = wartosc;
        ++rozmiar;
    }
}

// Dodaj na koniec tablicy
void dodaj_do_tablicy_na_koniec(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc) {
    if (rozmiar == pojemnosc) {
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < rozmiar; ++i)
            nowa_tablica[i] = tablica[i];
        delete[] tablica;
        tablica = nowa_tablica;
    }
    tablica[rozmiar++] = wartosc;
}

// Dodaj na wybrany indeks tablicy (jeśli za duży → dodaje na koniec)
void dodaj_do_tablicy_na_indeks(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc, int indeks) {
    if (indeks < 0) return;
    if (indeks > rozmiar) indeks = rozmiar;

    if (rozmiar == pojemnosc) {
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < indeks; ++i)
            nowa_tablica[i] = tablica[i];
        nowa_tablica[indeks] = wartosc;
        for (int i = indeks; i < rozmiar; ++i)
            nowa_tablica[i + 1] = tablica[i];
        delete[] tablica;
        tablica = nowa_tablica;
    }
    else {
        for (int i = rozmiar; i > indeks; --i)
            tablica[i] = tablica[i - 1];
        tablica[indeks] = wartosc;
    }
    ++rozmiar;
}

// Węzeł listy jednokierunkowej
struct Wezel {
    int wartosc;
    Wezel* nastepny;
    Wezel(int w) : wartosc(w), nastepny(nullptr) {}
};

// Dodaj na początek listy
void dodaj_do_listy_na_poczatek(Wezel*& pierwszy_el, int wartosc) {
    Wezel* nowy = new Wezel(wartosc);
    nowy->nastepny = pierwszy_el;
    pierwszy_el = nowy;
}

// Dodaj na koniec listy
void dodaj_do_listy_na_koniec(Wezel*& pierwszy_el, int wartosc) {
    Wezel* nowy = new Wezel(wartosc);
    if (!pierwszy_el) {
        pierwszy_el = nowy;
        return;
    }
    Wezel* temp = pierwszy_el;
    while (temp->nastepny)
        temp = temp->nastepny;
    temp->nastepny = nowy;
}

// Dodaj na wybrany indeks listy (jeśli za duży → dodaje na koniec)
void dodaj_do_listy_na_indeks(Wezel*& pierwszy_el, int wartosc, int indeks) {
    if (indeks < 0) return;

    if (indeks == 0) {
        dodaj_do_listy_na_poczatek(pierwszy_el, wartosc);
        return;
    }

    Wezel* temp = pierwszy_el;
    int i = 0;

    while (temp && i < indeks - 1) {
        temp = temp->nastepny;
        ++i;
    }

    if (!temp) {
        dodaj_do_listy_na_koniec(pierwszy_el, wartosc);
        return;
    }

    Wezel* nowy = new Wezel(wartosc);
    nowy->nastepny = temp->nastepny;
    temp->nastepny = nowy;
}

// Wyszukiwanie elementu w tablicy
int znajdz_w_tablicy(int* tablica, int rozmiar, int szukana_wartosc) {
    if (tablica == nullptr || rozmiar <= 0) return -1;

    for (int i = 0; i < rozmiar; ++i) {
        if (tablica[i] == szukana_wartosc)
            return i;
    }
    return -1;
}

// Wyszukiwanie elementu w liście
int znajdz_w_liscie(Wezel* pierwszy_el, int szukana_wartosc) {
    int indeks = 0;
    while (pierwszy_el) {
        if (pierwszy_el->wartosc == szukana_wartosc)
            return indeks;
        pierwszy_el = pierwszy_el->nastepny;
        ++indeks;
    }
    return -1;
}

int main() {
    const int N = 100000;

    int pojemnosc_tablicy = 40;
    int rozmiar_tablicy = 0;
    int* tablica = new int[pojemnosc_tablicy];

    Wezel* lista = nullptr;

    cout << "Dodawanie na poczatek:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_poczatek(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_poczatek(lista, i);
        }) << " ms\n";

    cout << "\nDodawanie na koniec:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_koniec(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_koniec(lista, i);
        }) << " ms\n";

    cout << "\nDodawanie na wybrany indeks (srodek):\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_indeks(tablica, rozmiar_tablicy, pojemnosc_tablicy, i, rozmiar_tablicy / 2);
        }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_indeks(lista, i, i / 2);
        }) << " ms\n";

    int szukana;
    cout << "\nPodaj wartosc do wyszukania: ";
    cin >> szukana;

    cout << "\nWyszukiwanie elementu " << szukana << ":\n";

    cout << "Tablica: " << zmierz_czas([&]() {
        int indeks = znajdz_w_tablicy(tablica, rozmiar_tablicy, szukana);
        cout << "Znaleziono na indeksie: " << indeks << endl;
        }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        int indeks = znajdz_w_liscie(lista, szukana);
        cout << "Znaleziono na indeksie: " << indeks << endl;
        }) << " ms\n";


    // Usuwanie pamięci na koniec
    delete[] tablica;
    while (lista) {
        Wezel* temp = lista;
        lista = lista->nastepny;
        delete temp;
    }

    return 0;
}
