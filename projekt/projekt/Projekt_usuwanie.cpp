#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

struct Wezel {
    int wartosc;
    Wezel* nastepny;
    Wezel(int w) : wartosc(w), nastepny(nullptr) {}
};

template <typename Funkcja>
long long zmierz_czas(Funkcja f) {
    auto start = high_resolution_clock::now();
    f();
    auto koniec = high_resolution_clock::now();
    return duration_cast<milliseconds>(koniec - start).count();
}

// --- Operacje na tablicy ---

void usun_poczatek_tablica(int*& tablica, int& rozmiar) {
    for (int i = 1; i < rozmiar; ++i)
        tablica[i - 1] = tablica[i];
    --rozmiar;
}

void usun_koniec_tablica(int& rozmiar) {
    --rozmiar;
}

void usun_indeks_tablica(int*& tablica, int& rozmiar, int indeks) {
    for (int i = indeks + 1; i < rozmiar; ++i)
        tablica[i - 1] = tablica[i];
    --rozmiar;
}

bool znajdz_tablica(int* tablica, int rozmiar, int wartosc) {
    for (int i = 0; i < rozmiar; ++i)
        if (tablica[i] == wartosc)
            return true;
    return false;
}

// --- Operacje na liście ---

void dodaj_na_koniec_listy(Wezel*& glowa, int wartosc) {
    Wezel* nowy = new Wezel(wartosc);
    if (!glowa) {
        glowa = nowy;
        return;
    }
    Wezel* temp = glowa;
    while (temp->nastepny)
        temp = temp->nastepny;
    temp->nastepny = nowy;
}

void usun_poczatek_lista(Wezel*& glowa) {
    if (!glowa) return;
    Wezel* temp = glowa;
    glowa = glowa->nastepny;
    delete temp;
}

void usun_koniec_lista(Wezel*& glowa) {
    if (!glowa) return;
    if (!glowa->nastepny) {
        delete glowa;
        glowa = nullptr;
        return;
    }
    Wezel* temp = glowa;
    while (temp->nastepny->nastepny)
        temp = temp->nastepny;
    delete temp->nastepny;
    temp->nastepny = nullptr;
}

void usun_indeks_lista(Wezel*& glowa, int indeks) {
    if (indeks == 0) {
        usun_poczatek_lista(glowa);
        return;
    }
    Wezel* temp = glowa;
    for (int i = 0; i < indeks - 1 && temp->nastepny; ++i)
        temp = temp->nastepny;
    if (!temp->nastepny) return;
    Wezel* do_usuniecia = temp->nastepny;
    temp->nastepny = do_usuniecia->nastepny;
    delete do_usuniecia;
}

bool znajdz_lista(Wezel* glowa, int wartosc) {
    while (glowa) {
        if (glowa->wartosc == wartosc)
            return true;
        glowa = glowa->nastepny;
    }
    return false;
}

// --- Test ---

void test_usuwanie_i_wyszukiwanie() {
    const int N = 20000;

    // Tablica
    int* tablica = new int[N];
    int rozmiar = N;
    for (int i = 0; i < N; ++i) tablica[i] = i;

    // Lista
    Wezel* lista = nullptr;
    for (int i = 0; i < N; ++i)
        dodaj_na_koniec_listy(lista, i);

    cout << "Usuwanie z początku:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_poczatek_tablica(tablica, rozmiar);
    }) << " ms\n";

    for (int i = 0; i < N; ++i) tablica[i] = i; rozmiar = N;
    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_poczatek_lista(lista);
    }) << " ms\n";

    // Ponowne tworzenie danych
    for (int i = 0; i < N; ++i) tablica[i] = i;
    rozmiar = N;
    for (int i = 0; i < N; ++i)
        dodaj_na_koniec_listy(lista, i);

    cout << "\nUsuwanie z końca:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_koniec_tablica(rozmiar);
    }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_koniec_lista(lista);
    }) << " ms\n";

    // Znów dodanie
    for (int i = 0; i < N; ++i) tablica[i] = i;
    rozmiar = N;
    for (int i = 0; i < N; ++i)
        dodaj_na_koniec_listy(lista, i);

    cout << "\nWyszukiwanie:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            znajdz_tablica(tablica, rozmiar, i);
    }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            znajdz_lista(lista, i);
    }) << " ms\n";

    delete[] tablica;
    while (lista) {
        Wezel* temp = lista;
        lista = lista->nastepny;
        delete temp;
    }
}

int main() {
    test_usuwanie_i_wyszukiwanie();
    return 0;
}
