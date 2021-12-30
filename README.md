# BakkesBallPredict
> First Project Using C++ So Comment In Issues Anyways You Would Make This Better.

## ABOUT

Simple Ball Prediction Plugin for Bakkesmod.
For this project i used c++ and the Bakkesmod SDK.

## INSTALLATION

To Install, Drag The DLL From ..AutoTrainingPlugin/x64/Release And Place It Into Your Bakkesmod Plugins Folder.

## IMPROVMENTS

Some Improvments I Could Make:
* The inbuilt "ball.PredictPosition" Function In BakkesMod Does Not Take Into Account That The Ball Has A Rotation Before Hitting The Ground. To Overcome This I/You Would Have To Recreate The "PredictPosition" Function And Pass A Rotational Value And Work Out The Maths.
* Clean Up The Code As Its My First C++ Project.
* Add A "Heading Towards Goal" Function And Make The Line A Different Colour If true. I Have Worked Out The Goal Post Coordanates So Would Just Need A Simple Check To See If "ProjectF(ball.PredictPosition.Location).X && .Y Are In AutoTrainingPlugin::GetBallLocation()"


