#include "pch.h"
#include "AutoTrainingPlugin.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "bakkesmod/wrappers/canvaswrapper.h"
#include "bakkesmod/wrappers/GameEvent/TutorialWrapper.h"


BAKKESMOD_PLUGIN(AutoTrainingPlugin, "Auto Training Plugin", "1.0", PERMISSION_ALL)

static float spawnlocation;


AutoTrainingPlugin::AutoTrainingPlugin()
{
}
AutoTrainingPlugin::~AutoTrainingPlugin()
{
}


void AutoTrainingPlugin::onLoad()
{
	totalstep = std::make_shared<int>(1000);
	divideby = std::make_shared<int>(1000);

	cvarManager->registerCvar("cl_soccar_totalsteps", "100", "Predict ball steps", true, true, 0, true, 1000).bindTo(totalstep);
	cvarManager->registerCvar("cl_soccar_divideby", "10", "Predict ball stepsize", true, true, 0, true, 1000).bindTo(divideby);


	this->Log("This is my first BakkesMod Plugin!!");

	this->LoadHooks();
	
}


void AutoTrainingPlugin::onUnload()
{
	gameWrapper->UnregisterDrawables();
}


void AutoTrainingPlugin::LoadHooks()
{
	//gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchStarted", std::bind(&AutoTrainingPlugin::GameStartedEvent, this, std::placeholders::_1));
	//gameWrapper->HookEvent("Function TAGame.AchievementManager_TA.HandleMatchStarted", std::bind(&AutoTrainingPlugin::GameStartedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventGoalScored", std::bind(&AutoTrainingPlugin::GoalScored, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameObserver_TA.EventCarHitBall", std::bind(&AutoTrainingPlugin::HitBall, this, std::placeholders::_1));


	gameWrapper->HookEvent("Function TAGame.Mutator_Freeplay_TA.Init", std::bind(&AutoTrainingPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.OnCarSpawned", std::bind(&AutoTrainingPlugin::carSpawn, this));
	
}


void AutoTrainingPlugin::OnFreeplayLoad(std::string name) {
	this->Log("Freeplay Loaded!");
	gameWrapper->RegisterDrawable(std::bind(&AutoTrainingPlugin::Render, this, std::placeholders::_1));
}	


void AutoTrainingPlugin::GoalScored(std::string name)
{
	this->Log("Goal Scored!");
}


void AutoTrainingPlugin::carSpawn() {
	this->Log("Car Spawned!");
	Vector location = GetCarLocation();
	spawnlocation = location.Y;
}


void AutoTrainingPlugin::HitBall(std::string name)
{
	this->Log("Ball Hit!");
	ServerWrapper game = gameWrapper->GetGameEventAsServer();
	BallWrapper ball = game.GetBall();
}


Vector AutoTrainingPlugin::GetCarLocation() { 
	ServerWrapper game = gameWrapper->GetGameEventAsServer();
	CarWrapper car = game.GetGameCar();

	Vector location = car.GetLocation();
	return location;
}


Vector AutoTrainingPlugin::GetBallLocation() {
	ServerWrapper game = gameWrapper->GetGameEventAsServer();
	BallWrapper ball = game.GetBall();
	
	Vector location = ball.GetLocation();
	return location;
}


std::vector<std::vector<float>> AutoTrainingPlugin::getGoalCoords() {
	// Vector Order:
	// bottom left:  865, 5120, 17
	// top left:     865, 5120, 650
	// bottom right: -855, 4915, 17
	// top right:    -855, 4915, 650

	std::vector<std::vector<float>> goalCoords{ 
		// first side
		{ 900.00, 5120.00, 17.00 },
		{ 900.00, 5120.00, 650.00 },
		{ -900.00, 5120.00, 17.00 },
		{ -900.00, 5120.00, 650 },
	
		// second side 
		{ -900.00, -5120.00, 17.00 },
		{ -900.00, -5120.00, 650.00 },
		{ 900.00, -5120.00, 17.00 },
		{ 900.00, -5120.00, 650 }
	};

	return goalCoords;
}


void AutoTrainingPlugin::Render(CanvasWrapper canvas) {
	/*
		Draw Line From Ball To Car
		#########################################################################################################################################################################################################
	*/
		// If the car hasnt been spawned yet, return
		if (spawnlocation == 0) {
			return;
		}

		// Get Ball Info 
		Vector blocation = GetBallLocation();
		ServerWrapper game = gameWrapper->GetGameEventAsServer();
		BallWrapper ball = game.GetBall();

		// Get Car Info
		Vector clocation = GetCarLocation();
	
		

		// Get Screen Coords Of Car, Ball
		Vector2F newclocation = canvas.ProjectF(clocation);
		Vector2F newblocation = canvas.ProjectF(blocation);

		// Draw A Line Between Car, Ball
		canvas.SetColor(255, 255, 255, 100);
		canvas.DrawLine(newclocation, newblocation, 3);

	/*
		#########################################################################################################################################################################################################
	*/





	/*
		Draw Line Over Goal Post
		#########################################################################################################################################################################################################
	*/

		/*

		// Get The Static Goal Coords
		std::vector<std::vector<float>> goalCoords = getGoalCoords();

		// If SpawnLocation > 0 reneder other side goal
		if (spawnlocation > 0) {
			Vector botLeft2 = { goalCoords[4][0], goalCoords[4][1], goalCoords[4][2] };
			Vector topLeft2 = { goalCoords[5][0], goalCoords[5][1], goalCoords[5][2] };
			Vector botRight2 = { goalCoords[6][0], goalCoords[6][1], goalCoords[6][2] };
			Vector topRight2 = { goalCoords[7][0], goalCoords[7][1], goalCoords[7][2] };

			Vector2F screenbotLeft2 = canvas.ProjectF(botLeft2);
			Vector2F screentopLeft2 = canvas.ProjectF(topLeft2);
			Vector2F screenbotRight2 = canvas.ProjectF(botRight2);
			Vector2F screentopRight2 = canvas.ProjectF(topRight2);

			canvas.DrawLine(screenbotLeft2, screentopLeft2, 4);
			canvas.DrawLine(screentopLeft2, screentopRight2, 4);
			canvas.DrawLine(screenbotRight2, screentopRight2, 4);
			canvas.DrawLine(screenbotLeft2, screenbotRight2, 4);
		}
		else if (spawnlocation < 0) {
			Vector botLeft = { goalCoords[0][0], goalCoords[0][1], goalCoords[0][2] };
			Vector topLeft = { goalCoords[1][0], goalCoords[1][1], goalCoords[1][2] };
			Vector botRight = { goalCoords[2][0], goalCoords[2][1], goalCoords[2][2] };
			Vector topRight = { goalCoords[3][0], goalCoords[3][1], goalCoords[3][2] };

			Vector2F screenbotLeft = canvas.ProjectF(botLeft);
			Vector2F screentopLeft = canvas.ProjectF(topLeft);
			Vector2F screenbotRight = canvas.ProjectF(botRight);
			Vector2F screentopRight = canvas.ProjectF(topRight);

			canvas.DrawLine(screenbotLeft, screentopLeft, 4);
			canvas.DrawLine(screentopLeft, screentopRight, 4);
			canvas.DrawLine(screenbotRight, screentopRight, 4);
			canvas.DrawLine(screenbotLeft, screenbotRight, 4);
		}

		*/

		
	/*
		#########################################################################################################################################################################################################
	*/





	/*
		Ball Trajectory
		#########################################################################################################################################################################################################
	*/
		// Stores Previous location
		Vector previousStep;
		
		// Calculates And Plots Lines 
		for (int i = 1; i < (*totalstep + 1); i++) {

			// First ittiration, Goes From Ball Location
			if (i == 1) {
				PredictionInfo predict = ball.PredictPosition(float(i)/ *divideby);
				previousStep = predict.Location;

				Vector2F newloc = canvas.ProjectF(predict.Location);
				canvas.SetColor(255, 0, 0, 200);
				canvas.DrawLine(newblocation, newloc, 7);
			}
			// Goes from Old Location -> NewLocation
			else {

				//ball.SetLocation(previousStep);
				PredictionInfo predict = ball.PredictPosition(float(i) / *divideby);
				Vector2 oldloc = canvas.Project(previousStep);

				Vector2 newloc = canvas.Project(predict.Location);
				canvas.SetColor(255, 0, 0, 200);
				canvas.DrawLine(oldloc, newloc, 7);
				canvas.DrawLine(oldloc.minus({ 2, 2 }), newloc.minus({ 2, 2 }), 7);
				canvas.DrawLine(oldloc.minus({ -2, -2 }), newloc.minus({ -2, -2 }), 7);

				previousStep = predict.Location;
			}
		}

		// NEED TO GO FROM BALLSTART -> NEWCLOCATION / NEWLOCATION -> PREDICT NEWLOCATION PREDICT. 

		// 
	/*
		#########################################################################################################################################################################################################
	*/


}


// A custom log wrapper. 
void AutoTrainingPlugin::Log(std::string msg)
{
	cvarManager->log("AutoTrainingPlugin: " + msg);
}