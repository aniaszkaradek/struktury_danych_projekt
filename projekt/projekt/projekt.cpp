#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

// mierzy czas
template <typename Funkcja>
long long zmierz_czas(Funkcja f) {
    auto start = high_resolution_clock::now();
    f();
    auto koniec = high_resolution_clock::now();
    return duration_cast<milliseconds>(koniec - start).count();
}


void dodaj_do_tablicy_na_poczatek(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc) 
{
    if (rozmiar == pojemnosc) 
    { 
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < rozmiar; ++i) 
        {
            nowa_tablica[i + 1] = tablica[i]; 
        }
        nowa_tablica[0] = wartosc; 
        delete[] tablica;
        tablica = nowa_tablica;
        ++rozmiar;
    }
}

void dodaj_do_tablicy_na_koniec(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc) {
    if (rozmiar == pojemnosc) 
    { 
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < rozmiar; ++i) 
        {
            nowa_tablica[i] = tablica[i];
        }
        delete[] tablica;
        tablica = nowa_tablica;
    }
    tablica[rozmiar++] = wartosc;
}

void dodaj_do_tablicy_na_indeks(int*& tablica, int& rozmiar, int& pojemnosc, int wartosc, int indeks) 
{
    if (indeks < 0 || indeks > rozmiar) return; 
    if (rozmiar == pojemnosc) 
    {
        pojemnosc *= 2;
        int* nowa_tablica = new int[pojemnosc];
        for (int i = 0; i < indeks; ++i) 
        {
            nowa_tablica[i] = tablica[i];
        }
        nowa_tablica[indeks] = wartosc;
        for (int i = indeks; i < rozmiar; ++i) 
        {
            nowa_tablica[i + 1] = tablica[i];
        }
        delete[] tablica;
        tablica = nowa_tablica;
        ++rozmiar;
    }
    else 
    {
        for (int i = rozmiar; i > indeks; --i) 
        {
            tablica[i] = tablica[i - 1];
        }
        tablica[indeks] = wartosc;
        ++rozmiar;
    }
}


struct Wezel 
{
    int wartosc;
    Wezel* nastepny;
    Wezel(int w) : wartosc(w), nastepny(nullptr) {}
};

void dodaj_do_listy_na_poczatek(Wezel*& pierwszy_el, int wartosc) 
{
    Wezel* nowy = new Wezel(wartosc);
    nowy->nastepny = pierwszy_el;
    pierwszy_el = nowy;
}

void dodaj_do_listy_na_koniec(Wezel*& pierwszy_el, int wartosc) 
{
    Wezel* nowy = new Wezel(wartosc);
    if (!pierwszy_el) 
    {
        pierwszy_el = nowy;
        return;
    }
    Wezel* temp = pierwszy_el;
    while (temp->nastepny) temp = temp->nastepny;
    temp->nastepny = nowy;
}

void dodaj_do_listy_na_indeks(Wezel*& pierwszy_el, int wartosc, int indeks) 
{
    if (indeks < 0) return;
    if (indeks == 0) 
    {
        dodaj_do_listy_na_poczatek(pierwszy_el, wartosc);
        return;
    }
    Wezel* temp = pierwszy_el;
    for (int i = 0; i < indeks - 1 && temp; ++i) temp = temp->nastepny;
    if (!temp) return;
    Wezel* nowy = new Wezel(wartosc);
    nowy->nastepny = temp->nastepny;
    temp->nastepny = nowy;
}


int main() {
    const int N = 20000; 

    int pojemnosc_tablicy = 40;
    int rozmiar_tablicy = 0;
    int* tablica = new int[pojemnosc_tablicy];

    Wezel* lista = nullptr;

    cout << "Dodawanie na poczatek: \n";

    cout << "Tablica: " << zmierz_czas([&]() 
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_poczatek(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " ms \n";

    cout << "Lista: " << zmierz_czas([&]() 
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_poczatek(lista, i);
        }) << " ms \n";

    // Pomiar czasu - dodawanie na koniec
    cout << "\n Dodawanie na koniec: \n";

    cout << "Tablica: " << zmierz_czas([&]() 
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_koniec(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " ms \n";

    cout << "Lista: " << zmierz_czas([&]() 
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_koniec(lista, i);
        }) << " ms \n";

    // Pomiar czasu - dodawanie na indeks
    cout << " \n Dodawanie na srodek: \n";

    cout << "Tablica: " << zmierz_czas([&]()
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_indeks(tablica, rozmiar_tablicy, pojemnosc_tablicy, i, rozmiar_tablicy / 2);
        }) << " ms\n";

    cout << "Lista: " << zmierz_czas([&]() 
        {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_indeks(lista, i, i / 2);
        }) << " ms \n";

    delete[] tablica;
    while (lista) 
    {
        Wezel* temp = lista;
        lista = lista->nastepny;
        delete temp;
    }

    return 0;
}
