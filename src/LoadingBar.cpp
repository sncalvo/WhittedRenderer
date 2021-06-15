#include "LoadingBar.hpp"

#include <iostream>

constexpr auto BARS = 15;
constexpr auto BAR = "#";

LoadingBar::LoadingBar(std::string title, int total) : _title(title), _total(total)
{
	_currentProgress = 0;
	std::cout << _title << "\n";
}

void LoadingBar::incrementProgress(int amount)
{
	_currentProgress += amount;

}

std::string _getLoadBar(int bars)
{
	std::string result = "";
	for (int i = 0; i < bars; i++)
	{
		result += BAR;
	}
	for (int i = bars; i < BARS; i++)
	{
		result += " ";
	}
	return result;
}

void LoadingBar::draw(std::string task)
{
	float percentage = _currentProgress / _total;
	std::cout << "\r" << task << " <" << _getLoadBar(percentage * BARS) << "> " << (int) (percentage * 100) << " %";
}
