#pragma once

class Fps
{
private:
	double timeFps;
	int timeStart;
	int countFlip;
	const double setFps = 60;
public:
	Fps();
	void Avg(); //fpsの平均を算出
	void Draw(); //fpsを表示
	void Wait(); //60fpsになるよう待機
};

