#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

bool wczytanie_nazwy(string nazwa, fstream& plik)
{
	cin >> nazwa;

	plik.open(nazwa.c_str());

	if (!plik)
	{
		cerr << "\nNie udalo sie otworzyc pliku " << nazwa << endl << "sprobuj jeszcze raz, pamietaj o koncowce *.txt" << endl << endl;
		return false;
	}
	return true;
}

void wczytanie_plikow(fstream& wejscie, fstream& wyjscie)
{
	string nazwa1, nazwa2;

	bool poprawnie = false;
	while (poprawnie != true)
	{
		cout << "Prosze podac nazwe pliku wejsciowego" << endl;
		poprawnie = wczytanie_nazwy(nazwa1, wejscie);
	}

	poprawnie = false;
	while (poprawnie != true)
	{
		cout << "Prosze podac nazwe pliku wyjsciowego" << endl;
		poprawnie = wczytanie_nazwy(nazwa2, wyjscie);
	}
}

bool czy_gnrmc(string linia)
{
	char znak;
	istringstream isstream(linia);
	ostringstream typlinii;
	while (isstream)
	{
		isstream >> znak;
		if (znak == '$')
		{
			for (int i = 0; i < 5; i++)
			{
				isstream >> znak;
				typlinii << znak;
			}
		}
	}
	if (typlinii.str() == "GNRMC")
		return true;
	else
		return false;
}

void utc(stringstream dane[14], fstream& wyjscie)
{
	char znak;
	int i = 0;
	wyjscie << "Data i czas pomiaru (UTC): ";
	while (dane[9].get(znak))
	{
		i++;
		if (i == 3 || i == 5)
			wyjscie << "/";
		wyjscie << znak;
	}
	wyjscie << " ";
	i = 0;
	while (dane[1].get(znak))
	{
		i++;
		if (i == 3 || i == 5)
			wyjscie << ":";
		if (znak == '.')
			break;
		wyjscie << znak;
	}
	wyjscie << endl;
}

void wariant1(stringstream dane[14], fstream& wyjscie)
{
	char znak;
	int i = 0;
	wyjscie << "Szerokosc geograficzna: ";
	while (dane[3].get(znak))
	{
		i++;
		if (i == 3)
			wyjscie << ".";
		if(i!=5)
			wyjscie << znak;
	}
	wyjscie << " " << dane[4].str() << endl;
	i = 0;
	wyjscie << "Dlugosc geograficzna: ";
	while (dane[5].get(znak))
	{
		i++;
		if (i == 4)
			wyjscie << ".";
		if (i == 1 && znak == '0');			//usuniecie zbednych zer
		else if (i != 6)
			wyjscie << znak;
	}
	wyjscie << " " << dane[6].str() << endl;
}

void wariant2(stringstream dane[14], fstream& wyjscie)
{
	char znak;
	int i = 0;
	wyjscie << "Szerokosc geograficzna: ";
	while (dane[3].get(znak))
	{
		i++;
		if (i == 3)
			wyjscie << " ";
		wyjscie << znak;
	}
	wyjscie << " " << dane[4].str() << endl;
	i = 0;
	wyjscie << "Dlugosc geograficzna: ";
	while (dane[5].get(znak))
	{
		i++;
		if (i == 4)
			wyjscie << " ";
		if (i == 1 && znak == '0');			//usuniecie zbednych zer
		else
			wyjscie << znak;
	}
	wyjscie << " " << dane[6].str() << endl;
}

void wariant3(stringstream dane[14], fstream& wyjscie)
{
	char znak;
	int i = 0;
	wyjscie << "Szerokosc geograficzna: ";
	while (dane[3].get(znak))
	{
		i++;
		if (i == 3 || i == 5)
			wyjscie << " ";
		else if (i == 8)
			wyjscie << ".";
		if (i != 5)
			wyjscie << znak;
	}
	wyjscie << " " << dane[4].str() << endl;
	i = 0;
	wyjscie << "Dlugosc geograficzna: ";
	while (dane[5].get(znak))
	{
		i++;
		if (i == 4 || i == 6)
			wyjscie << " ";
		else if (i == 9)
			wyjscie << ".";
		if (i == 1 && znak == '0');			//usuniecie zbednych zer
		else if (i != 6)
			wyjscie << znak;
	}
	wyjscie << " " << dane[6].str() << endl;
}

void wypisz_gnrmc(stringstream dane[14], int wariant, fstream& wyjscie)
{
	utc(dane, wyjscie);
	switch (wariant)
	{
		case 1:
			wariant1(dane, wyjscie);
			break;
		case 2:
			wariant2(dane, wyjscie);
			break;
		case 3:
			wariant3(dane, wyjscie);
			break;
		default:
			cout << "wkradl sie blad, nie mozna wypisac" << endl;
			break;
	}
	wyjscie << endl;
	for (int j = 0; j < 14; j++)		//czyszczenie tablicy
	{
		dane[j].str("");
		dane[j].clear();
	}
}

void pobierz_gnrmc(fstream& wejscie, fstream& wyjscie, int wariant)
{
	char znak;
	string linia;
	stringstream dane[14];
	while (getline(wejscie, linia))
	{
		istringstream czytanie(linia);
		if (czy_gnrmc(linia) == true)
		{

			while (czytanie.get(znak))
			{
				if (znak == '$')
				{
					int i = 0;
					while (i < 13)
					{
						czytanie >> znak;
						if (znak != ',')
							dane[i] << znak;
						else
							i++;
					}
				}
			}
			wypisz_gnrmc(dane, wariant, wyjscie);
		}
	}
}

int main()
{
	fstream wejscie, wyjscie;
	short int wariant;
	
	wczytanie_plikow(wejscie, wyjscie);
	cout << "Podaj wariant wypisania danych 1, 2 lub 3" << endl << "1 - stopnie" << endl << "2 - stopnie i minuty" << endl << "3 - stopnie, minuty i sekundy" << endl;
	cin >> wariant;
	while (wariant != 1 && wariant != 2 && wariant != 3)
	{
		cout << endl << "Zly numer wariantu, moze byc tylko 1, 2 lub 3. Wpisz poprawna wartosc" << endl;
		cin >> wariant;
	}
	pobierz_gnrmc(wejscie, wyjscie, wariant);

}

