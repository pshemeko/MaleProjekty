// PijaniMarynarze.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include "PijaniMarynarze.h"
//#include <cstdlib>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>



//wielowakowa aplikacja kilku pijanych marynarzy i jak dotkna sciany to zwiekrzaja licznik i trzeba zatrzymac rogram przy dokladnie kiln=ku(np.8 ) odbiciach
// moze zrobic ze jak dotkna to od nowa ida

int main()
{
    std::cout << "Hello World!\n"; 
	PijaniMarynarze pijani(10,11);
	pijani.start();
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln


//PijaniMarynarze::PijaniMarynarze()
//{
//}

const std::size_t STOP_WHEN = 22;

PijaniMarynarze::PijaniMarynarze(std::size_t HowManySailers, std::size_t WidthBoard)
	: mtx{}, sum(0), stop(false), size{ HowManySailers }, Width(WidthBoard)
{
	mtx.lock();
	//counters.reserve(a);
	std::vector<std::size_t> tmpCount(HowManySailers, Width/2);
	std::swap(counters, tmpCount);

	threads.reserve(HowManySailers * 2); // bo na prawo i na lewo
	
	for (size_t i = 0 ; i < HowManySailers; ++i)
	{
		//std::thread ten = std::thread(&PijaniMarynarze::changePosition, this, i, -1);
		threads.emplace_back(std::thread(&PijaniMarynarze::changePosition, this, i, -1)); // & PijaniMarynarze::changePosition
		threads.emplace_back( std::thread(&PijaniMarynarze::changePosition, this, i, 1));
	}
}

PijaniMarynarze::~PijaniMarynarze()
{
	stop = true;
	for (std::size_t i = 0; i < size * 2; ++i)
	{		
		threads[i].join();
	}
}

void PijaniMarynarze::show() const
{
	// czyszcze konsole
	system("CLS");

	for (std::size_t i = 0; i < size; ++i)
	{
		std::cout << '|' << 
			std::string(counters.at(i), '-') <<
			'*' << 
			std::string(Width - counters[i] - 1, '-') <<
			'|' << "  " << counters[i] << std::endl;
	}
	std::cout << "ilosc marynarzy= " << size << "  Marynarz 0 na pozycji=" << counters[0] << "  Wielkosc planszy=" << Width << std::endl;

}

void PijaniMarynarze::start() const
{
	mtx.unlock();
	while (!stop)
	{
		std::size_t sumCopy;
		{
			std::lock_guard<std::mutex>lk(mtx);
			sumCopy = sum;
		}
		show();
		std::cout << "Sum = " << sumCopy << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::cout << "Sum na koniec wynosi = " << sum << std::endl;
}

void PijaniMarynarze::changePosition(std::size_t counter_number, int value)
{
	while (!stop)
	{
		
		{
			std::lock_guard<std::mutex>lk(mtx);
			counters.at(counter_number) += value;
			if (counters.at(counter_number) >= Width)
			{
				counters.at(counter_number) = value;
				sum += 1;
			}
			else if (counters.at(counter_number) <= 0)
			{
				counters.at(counter_number) = 0;
				sum += 1;
			}
			if (sum == STOP_WHEN)
			{
				stop = true;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10 - 2*value));

	}
	
}
