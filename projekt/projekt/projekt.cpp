#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename Funkcja>
long long zmierz_czas(Funkcja f) {
    auto start = high_resolution_clock::now();
    f();
    auto koniec = high_resolution_clock::now();
    return duration_cast<microseconds>(koniec - start).count(); 
}

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

void usun_poczatek_tablicy(int*& tablica, int& rozmiar) {
    if (rozmiar > 0) {
        for (int i = 1; i < rozmiar; ++i)
            tablica[i - 1] = tablica[i];
        --rozmiar;
    }
}

void usun_koniec_tablicy(int*& tablica, int& rozmiar) {
    if (rozmiar > 0) {
        --rozmiar;
    }
}

void usun_indeks_tablicy(int*& tablica, int& rozmiar, int indeks) {
    if (indeks < 0 || indeks >= rozmiar) return;
    for (int i = indeks + 1; i < rozmiar; ++i)
        tablica[i - 1] = tablica[i];
    --rozmiar;
}

struct Wezel {
    int wartosc;
    Wezel* nastepny;
    Wezel(int w) : wartosc(w), nastepny(nullptr) {}
};

void dodaj_do_listy_na_poczatek(Wezel*& pierwszy_el, int wartosc) {
    Wezel* nowy = new Wezel(wartosc);
    nowy->nastepny = pierwszy_el;
    pierwszy_el = nowy;
}

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

void usun_poczatek_listy(Wezel*& pierwszy_el) {
    if (pierwszy_el) {
        Wezel* temp = pierwszy_el;
        pierwszy_el = pierwszy_el->nastepny;
        delete temp;
    }
}

void usun_koniec_listy(Wezel*& pierwszy_el) {
    if (!pierwszy_el) return;

    if (!pierwszy_el->nastepny) {
        delete pierwszy_el;
        pierwszy_el = nullptr;
        return;
    }

    Wezel* temp = pierwszy_el;
    while (temp->nastepny && temp->nastepny->nastepny)
        temp = temp->nastepny;

    Wezel* do_usuniecia = temp->nastepny;
    temp->nastepny = nullptr;
    delete do_usuniecia;
}

void usun_indeks_listy(Wezel*& pierwszy_el, int indeks) {
    if (indeks < 0) return;

    if (indeks == 0) {
        usun_poczatek_listy(pierwszy_el);
        return;
    }

    Wezel* temp = pierwszy_el;
    int i = 0;

    while (temp && i < indeks - 1) {
        temp = temp->nastepny;
        ++i;
    }

    if (!temp || !temp->nastepny) return;

    Wezel* do_usuniecia = temp->nastepny;
    temp->nastepny = temp->nastepny->nastepny;
    delete do_usuniecia;
}

int znajdz_w_tablicy(int* tablica, int rozmiar, int szukana_wartosc) {
    if (tablica == nullptr || rozmiar <= 0) return -1;

    for (int i = 0; i < rozmiar; ++i) {
        if (tablica[i] == szukana_wartosc)
            return i;
    }
    return -1;
}

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
    const int N = 50000;

    int pojemnosc_tablicy = 40;
    int rozmiar_tablicy = 0;
    int* tablica = new int[pojemnosc_tablicy];

    Wezel* lista = nullptr;

    cout << "Liczba elementow: 50000\n \n";

    cout << "Dodawanie na poczatek:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_poczatek(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_poczatek(lista, i);
        }) << " us\n";

    cout << "\nDodawanie na koniec:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_koniec(tablica, rozmiar_tablicy, pojemnosc_tablicy, i);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_koniec(lista, i);
        }) << " us\n";

    cout << "\nDodawanie na wybrany indeks:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_tablicy_na_indeks(tablica, rozmiar_tablicy, pojemnosc_tablicy, i, rozmiar_tablicy / 2);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            dodaj_do_listy_na_indeks(lista, i, i / 2);
        }) << " us\n";

    cout << "\nUsuwanie z poczatku:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_poczatek_tablicy(tablica, rozmiar_tablicy);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_poczatek_listy(lista);
        }) << " us\n";

    cout << "\nUsuwanie z konca:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_koniec_tablicy(tablica, rozmiar_tablicy);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_koniec_listy(lista);
        }) << " us\n";

    cout << "\nUsuwanie z wybranego indeksu:\n";
    cout << "Tablica: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_indeks_tablicy(tablica, rozmiar_tablicy, i / 2);
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        for (int i = 0; i < N; ++i)
            usun_indeks_listy(lista, i / 2);
        }) << " us\n";

    int szukana;
    cout << "\nPodaj wartosc do wyszukania: ";
    cin >> szukana;

    cout << "\nWyszukiwanie elementu " << szukana << ":\n";

    cout << "Tablica: " << zmierz_czas([&]() {
        int indeks = znajdz_w_tablicy(tablica, rozmiar_tablicy, szukana);
        cout << "Znaleziono na indeksie: " << indeks << endl;
        }) << " us\n";

    cout << "Lista: " << zmierz_czas([&]() {
        int indeks = znajdz_w_liscie(lista, szukana);
        cout << "Znaleziono na indeksie: " << indeks << endl;
        }) << " us\n";

    system("python C:\\Users\\Dell\\Desktop\\studia\\sem3\\struktury_danych\\struktury_danych_projekt\\projekt\\projekt\\wykresy.py");

    delete[] tablica;
    while (lista) {
        Wezel* temp = lista;
        lista = lista->nastepny;
        delete temp;
    }

    return 0;
}
