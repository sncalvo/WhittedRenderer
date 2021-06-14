
#include <string>

class LoadingBar
{
private:
	std::string _title;
	float _total;
	float _currentProgress;
	std::string loadBar;
public:
	LoadingBar(std::string title, int total);
	void incrementProgress(int amount);
	void draw(std::string task = "");
};
