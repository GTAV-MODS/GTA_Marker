#include "script.h"
#include <cmath>
#include <string>

static void drawText3DMarker(float coordX, float coordY, float playerZ, char* text)
{
    float x, y;
    if (GRAPHICS::_WORLD3D_TO_SCREEN2D(coordX, coordY, playerZ, &x, &y))
    {
        UI::SET_TEXT_SCALE(0.33f, 0.33f);
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

static float getDistance(float a_X, float a_Y, float a_Z, float b_X, float b_Y, float b_Z)
{
    float x = a_X - b_X;
    float y = a_Y - b_Y;
    float z = a_Z - b_Z;
    float sumDist = x * x + y * y + z * z;
    float res = std::sqrt(sumDist);
    return res;
}

static void showPlayerMarker(const Vector3& playerPos, const Vector3& coord, float distance, char* text)
{
    if (distance > 999)
    {
        float roundOverKM = std::round(distance) * 0.001f;
        sprintf(text, "V\n %.1f km", roundOverKM);
    }
    else
    {
        sprintf(text, "V\n %d m", static_cast<int>(std::round(distance)));
    }

    drawText3DMarker(coord.x, coord.y, playerPos.z, text);
}

void ScriptMain()
{
    char text[100] = "";
    int blip = 0;
    bool foundBlip = false;
    bool placingMarker = false;
    Vector3 markerPos;
    bool markerActive = false;

    while (true)
    {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, 51)) // Touche "E"
        {
            if (!markerActive)
            {
                placingMarker = true;

                Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
                Vector3 direction = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());
                markerPos.x = playerPos.x + (direction.x * 10.0f);
                markerPos.y = playerPos.y + (direction.y * 10.0f);
                markerPos.z = playerPos.z + (direction.z * 10.0f);

                float distance = getDistance(playerPos.x, playerPos.y, playerPos.z, markerPos.x, markerPos.y, markerPos.z);
                sprintf(text, "%.1f m", distance);

                markerActive = true;
            }
            else
            {
                markerActive = false;
            }
        }

        if (markerActive)
        {
            Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
            float distance = getDistance(playerPos.x, playerPos.y, playerPos.z, markerPos.x, markerPos.y, markerPos.z);
            sprintf(text, "%.1f m", distance);

            // Afficher le marqueur à l'endroit où le joueur regarde avec la distance
            drawText3DMarker(markerPos.x, markerPos.y, markerPos.z, text);
        }

        if (!foundBlip)
        {
            // Loop through the blips to find if it corresponds to the player blip mission
            for (int i = 0; i < 826; i++)
            {
                blip = UI::GET_FIRST_BLIP_INFO_ID(i);
                bool checkIfBlipHaveRoute = (UI::DOES_BLIP_EXIST(blip) && UI::_0xDD2238F57B977751(blip));
                bool isWaypointFound = (i == 8 && UI::DOES_BLIP_EXIST(UI::GET_FIRST_BLIP_INFO_ID(i)));

                // Check if the blip waypoint is found, then exit the loop and show the waypoint marker distance
                if (isWaypointFound || checkIfBlipHaveRoute)
                {
                    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
                    Vector3 coord = UI::GET_BLIP_INFO_ID_COORD(blip);
                    float distance = getDistance(playerPos.x, playerPos.y, playerPos.z, coord.x, coord.y, coord.z);
                    showPlayerMarker(playerPos, coord, distance, text);
                }
            }
        }
        else
        {
            if (!UI::DOES_BLIP_EXIST(blip))
            {
                foundBlip = false;
            }
            else
            {
                Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
                Vector3 coord = UI::GET_BLIP_INFO_ID_COORD(blip);
                float distance = getDistance(playerPos.x, playerPos.y, playerPos.z, coord.x, coord.y, coord.z);
                showPlayerMarker(playerPos, coord, distance, text);
            }
        }

        WAIT(0);
    }
}