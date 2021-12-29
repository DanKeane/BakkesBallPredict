#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"

#pragma comment( lib, "pluginsdk.lib" )

// Questions? 
// https://linktr.ee/pidgy

class AutoTrainingPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::shared_ptr<int> totalstep;
	std::shared_ptr<int> divideby;
	void Log(std::string msg);

public:
	AutoTrainingPlugin();
	~AutoTrainingPlugin();

	virtual void onLoad();
	virtual void onUnload();
	void LoadHooks();
	void OnFreeplayLoad(std::string name);
	void GoalScored(std::string name);
	void carSpawn();
	void HitBall(std::string name);
	Vector GetCarLocation();
	Vector GetBallLocation();
	std::vector<std::vector<float>> getGoalCoords();
	void Render(CanvasWrapper canvas);
};