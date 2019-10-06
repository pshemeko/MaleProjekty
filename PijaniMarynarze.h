#pragma once
#include <mutex>
#include <vector>

class PijaniMarynarze
{
public:
	PijaniMarynarze(std::size_t a, std::size_t WidthBoard);
	~PijaniMarynarze();
	// wyswielta
	void show() const;
	//by uruchomic program
	void start() const;


private:

	void changePosition(std::size_t counter, int value);

	mutable std::mutex mtx;
	// licznik ile razu dotknieto sciany
	std::size_t sum;

	
	// wektor licznikow dla marynarzy
	std::vector<std::size_t> counters;

	// wektor w¹tków
	std::vector<std::thread> threads;


	const std::size_t size; // ilosc marynarzy

	const std::size_t Width;
	bool stop;

};

