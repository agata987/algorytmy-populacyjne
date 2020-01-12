#include "GeneticAlg.h"

void GeneticAlg::run()
{
	srand(time(NULL));

	przygotujPopulacje();

	for (int i = 0; i < liczbaPokolen; i++)
	{
		krzyzowanie();

		mutacje();

		//OX_b = false;
		sort(populacja.begin(), populacja.end());

		usunNajgorsze();
	}

	// wyswietlenie najlepszego osobnika
	cout << "najlepszy wynik: ";
	populacja[0].wyswietl();
}

void GeneticAlg::DisplayProperties()
{
	cout << "\nwielkosc populacji: " << wielkoscPopulacji << endl;
	cout << "liczba pokolen: " << liczbaPokolen << endl;
	cout << "prawdopodobienstwo krzyzowania: " << prawdopodobienstwoKrzyzowania << endl;
	cout << "prawdopodobienstwo mutacji: " << prawdopodobienstwoMutacji << endl;
}

void GeneticAlg::krzyzowanie() 
{
	if (OX_b)
		OX();
	else
		PMX();
}

void GeneticAlg::OX()
{
	vector<Osobnik> potomkowie, rodzice;
	Osobnik potomek;

	int liczbaPar = wielkoscPopulacji / 2;

	// randA -odkad kopiowac, randB - dokad
	int randA, randB;

	for (int i = 0; i < liczbaPar; i++)
	{
		int tmp = rand() % 100;

		if (tmp < prawdopodobienstwoKrzyzowania) {

			// losujemy rodzicow, ktorzy nie maja jeszcze dzieci
			rodzice = losujRodzicow();

			do {
				randA = rand() % numberOfcities;
				randB = rand() % numberOfcities;
			} while (randA == randB);

			if (randA > randB)
				swap(randA, randB);

			//kopiujemy od rodzic1
			vector<int> odRodzic1;

			for (int j = randA; j != randB; j++)
				odRodzic1.push_back(rodzice.at(0).path.at(j));


			//kopiujemy reszte od rodzic2

			//kopiujemy koncowke
			vector<int> poRodzic1;

			int index = randB;
			int wartosc = 0;

			// bierzemy wartosc od drugiego rodzica
			for (int j = randB; j != rodzice.at(1).path.size(); j++)
			{
				wartosc = rodzice.at(1).path.at(index);

				while (contains(odRodzic1, wartosc) || contains(poRodzic1, wartosc))
				{
					index++;

					if (index == rodzice.at(1).path.size())
						index = 0;

					wartosc = rodzice.at(1).path.at(index);
				}

				// jezeli nie ma tej wartosci w poRodzic1 ani w odRodzic1 to dodajemy ja do poRodzic1 
				poRodzic1.push_back(wartosc);
			}

			//kopiujemy poczatek
			vector<int> przedRodzic1;
			index = 0;

			for (int j = 0; j != randA; j++)
			{
				// bierzemy wartosc od drugiego rodzica
				int wartosc = rodzice.at(1).path.at(index);

				// jezeli nie ma wartosci w: odRodzic1, poRodzic1, przedRodzic1
				while (contains(odRodzic1, wartosc) || contains(poRodzic1, wartosc) || contains(przedRodzic1, wartosc))
				{
					index++;
					wartosc = rodzice.at(1).path.at(index);
				}

				przedRodzic1.push_back(wartosc);

			}

			// laczymy w calosc, sklejamy wektory
			potomek.path = przedRodzic1;
			potomek.path.insert(potomek.path.end(), odRodzic1.begin(), odRodzic1.end());
			potomek.path.insert(potomek.path.end(), poRodzic1.begin(), poRodzic1.end());

			potomek.wartosc = funkcjaCelu(potomek.path);
			potomkowie.push_back(potomek);

			//DO USUNIECIA
			//cout << "\nrodzic1:" << endl;
			//rodzice.at(0).wyswietl();
			//cout << "\nrodzic2:" << endl;
			//rodzice.at(1).wyswietl();
			//cout << "\npotomek:" << endl;
			//potomek.wyswietl();

		}
		
	}

	// dodajemy potomkow do populacji
	populacja.insert(populacja.end(), potomkowie.begin(), potomkowie.end());

	// ustawiamy cala populacje jako taka ktora nie ma dzieci dla nast. krzyzowan
	for (vector<Osobnik>::iterator it = populacja.begin(); it != populacja.end(); it++)
		it->maPotomka = false;
}

void GeneticAlg::PMX()
{
	vector<Osobnik> potomkowie, rodzice;
	Osobnik potomek;

	int liczbaPar = wielkoscPopulacji / 2;

	// randA -odkad kopiowac, randB - dokad
	int randA, randB;

	for (int i = 0; i < liczbaPar; i++)
	{
		int tmp = rand() % 100;

		if (tmp < prawdopodobienstwoKrzyzowania) 
		{

			do {
				randA = rand() % numberOfcities;
				randB = rand() % numberOfcities;
			} while (randA == randB);

			if (randA > randB)
				swap(randA, randB);


			rodzice = losujRodzicow();

			potomek.path.clear();
			for (int i = 0; i < numberOfcities; i++)
				potomek.path.push_back(-1);


			//kopoujemy do potomka od rodzica1
			for (int j = randA; j != randB; j++)
				potomek.path[j] = rodzice[0].path[j];

			// wstawiamy w odpowiednie miejsca miasta od rodzica 2 na indeksach skopiowanych od rodzica 1
			int gdzie, co, temp;
			for (int j = randA; j != randB; j++)
			{
				co = rodzice[1].path[j];
				if (!contains(potomek.path, co))
				{

					int wartoscR0 = rodzice[0].path[j]; // wartosc rodzica 0 pod indeksem j
					gdzie = getIndexOf(rodzice[1].path, wartoscR0); // indeks gdzie ta wartosc sie znajduje u rodzica 1

					while (gdzie >= randA && gdzie < randB)
					{
						// wartosc rodzica 0
						temp = rodzice[0].path[gdzie];
						// indeks rodzica 1
						gdzie = getIndexOf(rodzice[1].path, temp);
					}

					potomek.path[gdzie] = co;
				}
			}

			for (int i = 0; i < numberOfcities; i++)
			{
				if (potomek.path[i] == -1)
					potomek.path[i] = rodzice[1].path[i];
			}

			potomek.wartosc = funkcjaCelu(potomek.path);
			potomkowie.push_back(potomek);

			//DO USUNIECIA

			//cout << "\n-----------------------" << endl;
			//cout << "\nrodzic1:" << endl;
			//rodzice.at(0).wyswietl();
			//cout << "\nrodzic2:" << endl;
			//rodzice.at(1).wyswietl();
			//cout << "\npotomek:" << endl;
			//potomek.wyswietl();

		}
		
	}

	// dodajemy potomkow do populacji
	populacja.insert(populacja.end(), potomkowie.begin(), potomkowie.end());

	// ustawiamy cala populacje jako taka ktora nie ma dzieci dla nast. krzyzowan
	vector<Osobnik>::iterator it;

	for (it = populacja.begin(); it != populacja.end(); it++)
		it->maPotomka = false;
}

vector<Osobnik> GeneticAlg::losujRodzicow()
{
	Osobnik rodzic1, rodzic2;
	int rodzic1_int, rodzic2_int;
	vector<Osobnik> rodzice;

	do {
		// losujemy rodzicow
		do {
			rodzic1_int = rand() % wielkoscPopulacji;
			rodzic2_int = rand() % wielkoscPopulacji;
		} while (rodzic1_int == rodzic2_int);

		rodzic1 = populacja.at(rodzic1_int);
		rodzic2 = populacja.at(rodzic2_int);

		//sprawdzamy czy wylosowani rodzice maja juz potomkow
	} while (rodzic1.maPotomka || rodzic2.maPotomka);

	rodzice.push_back(rodzic1);
	rodzice.push_back(rodzic2);

	populacja.at(rodzic1_int).maPotomka = true;
	populacja.at(rodzic2_int).maPotomka = true;

	return rodzice;
}

void GeneticAlg::set_liczbaPokolen(int liczbaPokolen)
{
	this->liczbaPokolen = liczbaPokolen;
}

void GeneticAlg::mutacje()
{

	for (int i = 0; i < wielkoscPopulacji; i++)
	{
		int tmp = rand() % 100;
		int randA, randB;

		if (tmp < prawdopodobienstwoMutacji)
		{

			do {
				randA = rand() % numberOfcities;
				randB = rand() % numberOfcities;
			} while (randA == randB);

			if (randA > randB)
				swap(randA, randB);


			for (int j = randA; j < randB/2; j++)
				swap(populacja[i].path[j], populacja[i].path[randB - j]);

			populacja[i].wartosc = funkcjaCelu(populacja[i].path);
		}
	}
}

void GeneticAlg::usunNajgorsze()
{

	while (populacja.size() > wielkoscPopulacji)
		populacja.pop_back();
}