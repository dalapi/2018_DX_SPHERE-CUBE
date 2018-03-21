#include "Timer.h"
#include "Number.h"
#include "GameScene.h"

Timer* Timer::instance = nullptr;

Timer::Timer()
{
	time = count = second = 0;
	bEnd = 0;
}

Timer::~Timer()
{

}

void Timer::Init()
{
	time = count = second = 0;
}

void Timer::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}
void Timer::Update()
{
	count++;
	second = count / 60;
	//time = second;
	time = TIME_LIMIT - second;

	// 
	if (time <= 0) { bEnd = 1; }
}

void Timer::Draw(int dig, float x, float y)
{
	int nTime = time;

	for (int i = dig - 1; i >= 0; i--)
	{
		int n = nTime % 10;

		Number::DrawNumber(n, x + (NUMBER_WIDTH-3) * i, y);

		nTime /= 10;
	}
}

void Timer::GUIUpdate()
{
	ImGui::Text("Timer");
	ImGui::Text("time %2d count %2d second %2d", time, count, second);
}