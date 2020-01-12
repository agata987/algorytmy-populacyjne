#include "Algorithm.h"

void Algorithm::readFromFile()
{
	string sciezka;
	cout << "Podaj sciezke do pliku: ";
	cin >> sciezka;

	fstream plik;
	plik.open(sciezka, ios::in);

	if (plik.good() == false)
	{
		cout << "Blad otwarcia pliku." << endl;
	}
	else
	{
		// wczesniej istnial jakis graf
		if (numberOfcities != -1)
		{
			for (int i = 0; i < numberOfcities; i++)
				delete[] matrix[i];

			delete[] matrix;
		}

		plik >> name;
		plik >> numberOfcities;

		matrix = new int*[numberOfcities];

		for (int i = 0; i < numberOfcities; i++)
			matrix[i] = new int[numberOfcities];

		int  value;
		for (int i = 0; i < numberOfcities; i++)
		{
			for (int j = 0; j < numberOfcities; j++)
			{
				plik >> value;
				matrix[i][j] = value;
			}
		}
	}
	plik.close();
}

void Algorithm::generateRandom(int size)
{
	// wczesniej istnial jakis graf
	if (numberOfcities != -1)
	{
		for (int i = 0; i < numberOfcities; i++)
			delete[] matrix[i];

		delete[] matrix;
	}

	numberOfcities = size;
	name = "none";

	matrix = new int*[numberOfcities];

	for (int i = 0; i < numberOfcities; i++)
		matrix[i] = new int[numberOfcities];
	
	srand(time(NULL));

	for (int i = 0; i < numberOfcities; i++)
		for (int j = 0; j < numberOfcities; j++)
			if (i != j)
				matrix[i][j] = rand() % 101 + 1;
			else
				matrix[i][j] = -1;
		
}

void Algorithm::print()
{
	if (numberOfcities == -1)
		cout << "Brak grafu." << endl;
	else
	{
		cout << " " << "\t";
		for (int i = 0; i < numberOfcities; i++)
			std::cout << i << "\t";
		cout << endl;

		for (int i = 0; i < numberOfcities; i++)
		{
			cout << i << "\t";
			for (int j = 0; j < numberOfcities; j++)
				std::cout << matrix[i][j] << "\t";

			cout << std::endl;
		}
	}
}

bool Algorithm::contains(vector<int> v, int value)
{
	
	for (vector<int> ::iterator it = v.begin(); it != v.end(); it++)
		if (*it == value)
			return true;

	return false;
}

int Algorithm::getIndexOf(vector<int> v, int value)
{
	for (int i = 0; i < v.size(); i++)
		if (v[i] == value)
			return i;

	return -1;
}

void Algorithm::setWielkoscPopulacji(int wielkosc)
{
	wielkoscPopulacji = wielkosc;
}

void Algorithm::przygotujPopulacje()
{
	vector<int> path;

	for (int i = 0; i < numberOfcities; i++)
		path.push_back(i);

	srand(time(NULL));

	Osobnik osobnik;

	populacja.clear();
	for (int i = 0; i < wielkoscPopulacji; i++)
	{
		random_shuffle(path.begin(), path.end());

		osobnik.path = path;
		osobnik.wartosc = funkcjaCelu(osobnik.path);

		populacja.push_back(osobnik);
	}
}

int Algorithm::funkcjaCelu(vector<int> path)
{
	int sum = 0;
	for (vector<int>::iterator it = path.begin(); it != prev(path.end()); it++)
		sum += matrix[*it][*(next(it, 1))];

	sum += matrix[path.back()][path.front()];

	return sum;
}

void Algorithm::wyswietlPopulacje()
{
	int number = 1;
	for (Osobnik os : populacja)
	{
		cout << number << "|";
		os.wyswietl();
		cout << endl;
		number++;
	}
}