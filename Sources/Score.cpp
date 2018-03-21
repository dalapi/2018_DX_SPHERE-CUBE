#include "Score.h"
#include <string.h>
#include <stdio.h>
#include "Number.h"

#define _CRT_SECURE_NO_WARNINGS 1

Score* Score::instance = nullptr;

Score::Score()
{
}


Score::~Score()
{
}

void Score::Init()
{
	nScore = 0;
}

void Score::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Score::Plus(int Plus)
{
	nScore += Plus;
}

void Score::Draw(int d, float x, float y)
{
	char buff[256];

	int dig = d;
	bool bZero = 1;

	// スコア数字
	if (d <= 0)
	{
		sprintf_s(buff, "%d", nScore);
		dig = strlen(buff);
	}
	else
	{
		//カンスト値を作る
		int max = 9;
		int pow10 = 10;
		for (int i = 1; i < dig; i++)
		{
			max += 9 * pow10;
			pow10 *= 10;
		}
	}

	int nSaveScore = nScore;

	// score == 1234567
	for (int i = dig - 1; i >= 0; i--)
	{
		// n = scoreを10で割った余り
		int n = nSaveScore % 10;

		Number::DrawNumber(n, x + (NUMBER_WIDTH-3) * i, y);
		nSaveScore /= 10;

		if (!bZero && nSaveScore == 0)
		{
			return;
		}
		else if (bZero)
		{
			//	DrawNumber(0, x + NUMBER_WIDTH, y);
		}
	}
}

int Score::Get()
{
	return nScore;
}

void Score::GUIUpdate()
{
	ImGui::Text("Score %d", nScore);
}