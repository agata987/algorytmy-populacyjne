#pragma once
#include<string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

struct Osobnik
{
	vector<int> path;
	int wartosc = -1;
	bool maPotomka = false;

	bool operator < (const Osobnik& os) const
	{
		return wartosc < os.wartosc;
	}

	void wyswietl()
	{
		vector<int> ::iterator it;
		for (int i = 0; i < path.size(); i++)
			cout << path[i] << " ";

		cout << "		" << wartosc;
	}
};

class Algorithm
{
protected:
	//	Graph
	int **matrix;
	int numberOfcities;
	string name;

	//	Population
	int wielkoscPopulacji;
	vector<Osobnik> populacja;

	// Funkcje pomocnicze
	bool contains(vector<int>, int value);
	int getIndexOf(vector<int> v, int value);
	int funkcjaCelu(vector<int> path);
public:
	Algorithm() {
		name = "none";
		numberOfcities = -1;

		wielkoscPopulacji = 30;
	};
	~Algorithm() = default;

	void readFromFile();
	void generateRandom(int size);
	void print();

	void setWielkoscPopulacji(int wielkosc);
	void przygotujPopulacje();
	void wyswietlPopulacje();

	virtual void run() = 0;
	virtual void DisplayProperties() = 0;
};

