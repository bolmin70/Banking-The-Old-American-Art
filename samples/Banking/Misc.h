#include "script.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <Windows.h>
#include <shlwapi.h>

#pragma once
class Misc
{
public:

	static Hash getCurrentPlayerWeapon();

	static void createPrompt(Prompt &prompt, const char* input, const char* text, int press_type);

	static void createPrompt(Prompt& prompt, const char* input, const char* text, int press_type, int group);

	static void removeBlipsSafe(Blip blip);
	
	static Vector3 getPlayerPos();

	static float getGroundPos(Vector3 originalPos);

	static Vector3 getRandomPositionInRange(Vector3 center, int radius);

	static Vector3 getRandomPedPositionInRange(Vector3 source, int radius);


	static Ped createPedOnHorse(char* modelName, Ped horse, int seatIndex);

	static Ped createPed(const char* modelName, Vector3 pos, float heading);

	static Vector3 toVector3(float x, float y, float z);

	static Ped createVehicle(char* modelName, Vector3 pos, float heading);

	static Object createProp(char* model, Vector3 position, float heading, bool isStatic, bool isVisible);

	static Object createProp(Hash model, Vector3 position, float heading, bool isStatic, bool isVisible);

	static void Misc::showSubtitle(const char* text);

	static void Misc::drawText(const char* text, float x, float y, int r, int g, int b, int a, bool centered, float sx, float sy);

	static void Misc::drawTextSmall(const char* text, float x, float y, int r, int g, int b, int a, bool centered, float sx, float sy);

	static void Misc::drawSprite(char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a);


};

