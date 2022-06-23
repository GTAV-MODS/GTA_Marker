#include "script.h"
#include <cmath>
#include <string>

/*Copyright for (C) Alexander Blade (NATIVE TRAINER BASE CODE)*/
/*Script made by SuperCoolNinja*/


void drawText3DMarker(float coordX, float coordY, float playerZ, char* text)
{
	float x, y;
	if (GRAPHICS::_WORLD3D_TO_SCREEN2D(coordX, coordY, playerZ, &x, &y))
	{
		UI::SET_TEXT_SCALE(0.3 + 0.03, 0.3 + 0.03);
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_PROPORTIONAL(TRUE);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 55);
		UI::SET_TEXT_EDGE(2, 0, 0, 0, 150);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::SET_TEXT_CENTRE(TRUE);
		UI::_ADD_TEXT_COMPONENT_STRING(text);
		UI::_DRAW_TEXT(x, y);
	}
}

float getDistance(float a_X, float a_Y, float a_Z, float b_X, float b_Y, float b_Z)
{
	auto x = a_X - b_X;
	auto y = a_Y - b_Y;
	auto z = a_Z - b_Z;
	auto sumDist = x * x + y * y + z * z;
	auto res = std::sqrt(sumDist);
	return res;
}


//Main entry :
void ScriptMain()
{
	char bufferOne[100];
	char bufferTwo[100];

	int timerOptimize = 1000;

	while (true)
	{
		timerOptimize = 1000;

		auto waypoint = UI::GET_FIRST_BLIP_INFO_ID(8);
		if (UI::DOES_BLIP_EXIST(waypoint))
		{
			auto playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			auto coord = UI::GET_BLIP_INFO_ID_COORD(waypoint);
			auto distance = getDistance(playerPos.x, playerPos.y, playerPos.z, coord.x, coord.y, coord.z);

			if (distance > 999)
			{
				auto roundOverKM = std::round(distance) * std::pow(10, -3);
				sprintf(bufferOne, "V\n %.1lf km", roundOverKM);
			}
			else sprintf(bufferOne, "V\n %ld m", (int)std::round(distance));
			
			drawText3DMarker(coord.x, coord.y, playerPos.z, bufferOne);
			timerOptimize = 0;
		}

		WAIT(timerOptimize);
	}
}
