#include "script.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <Windows.h>
#include <shlwapi.h>
#include "Misc.h"




Hash Misc::getCurrentPlayerWeapon() {
	Hash current;
	WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &current, 0, 0, 0);

	return current;
}



void Misc::createPrompt(Prompt &prompt, const char* input, const char* text, int press_type) //press type 0 - hold mode, 1 - long hold mode, 2 - click mod, 3 - autofill

{
	prompt = HUD::_UIPROMPT_REGISTER_BEGIN(); 
	HUD::_UIPROMPT_SET_CONTROL_ACTION(prompt, MISC::GET_HASH_KEY(input)); 
	HUD::_UIPROMPT_SET_TEXT(prompt, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text)); 


	switch (press_type) {
	case 0:
		HUD::_UIPROMPT_SET_HOLD_MODE(prompt, 1);
		break;
	case 1:
		HUD::_UIPROMPT_SET_STANDARDIZED_HOLD_MODE(prompt, 1);
		break;
	case 2:
		HUD::_UIPROMPT_SET_STANDARD_MODE(prompt, 1);
		break;
	case 3:
		HUD::_UIPROMPT_SET_MASH_AUTO_FILL_MODE(prompt, 100, 3000);
	};



	//HUD::_UIPROMPT_SET_GROUP(Prompt_Test, 268566632, 0);

	HUD::_UIPROMPT_REGISTER_END(prompt); 


	HUD::_UIPROMPT_SET_ENABLED(prompt, 0); 
	HUD::_UIPROMPT_SET_VISIBLE(prompt, 0); 


}

void Misc::createPrompt(Prompt& prompt, const char* input, const char* text, int press_type, int group) //press type 0 - hold mode, 1 - long hold mode, 2 - click mod, 3 - autofill
{
	prompt = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_UIPROMPT_SET_CONTROL_ACTION(prompt, MISC::GET_HASH_KEY(input));
	HUD::_UIPROMPT_SET_TEXT(prompt, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text));


	switch (press_type) {
	case 0:
		HUD::_UIPROMPT_SET_HOLD_MODE(prompt, 1);
		break;
	case 1:
		HUD::_UIPROMPT_SET_STANDARDIZED_HOLD_MODE(prompt, 1);
		break;
	case 2:
		HUD::_UIPROMPT_SET_STANDARD_MODE(prompt, 1);
		break;
	case 3:
		HUD::_UIPROMPT_SET_MASH_AUTO_FILL_MODE(prompt, 100, 3000);
	};



	HUD::_UIPROMPT_SET_GROUP(prompt, group, 0);

	HUD::_UIPROMPT_REGISTER_END(prompt);


	HUD::_UIPROMPT_SET_ENABLED(prompt, 0);
	HUD::_UIPROMPT_SET_VISIBLE(prompt, 0);


}


void Misc::removeBlipsSafe(Blip blip)
{
	if (MAP::DOES_BLIP_EXIST(blip)) {
		MAP::REMOVE_BLIP(&blip);
	}
}

Vector3 Misc::getPlayerPos()
{



	return ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0);
}

float Misc::getGroundPos(Vector3 originalPos)
{
	float groundZ;
	MISC::GET_GROUND_Z_FOR_3D_COORD(originalPos.x, originalPos.y, originalPos.z + 30, &groundZ, false);
	return groundZ;
}

Vector3 Misc::getRandomPositionInRange(Vector3 center, int radius)
{
	int x = rndInt((int)center.x - radius, (int)center.x + radius + 1);
	int topOrBottom = rndInt(0, 2) == 1 ? 1 : -1;
	double y = topOrBottom * sqrt(pow(radius, 2) - pow(x - center.x, 2)) + center.y;

	Vector3 output;
	output.x = x;
	output.y = (float)y;
	output.z = getGroundPos(output);
	return output;
}

Vector3 Misc::getRandomPedPositionInRange(Vector3 source, int radius)
{
	Vector3 position = getRandomPositionInRange(source, radius);
	PATHFIND::GET_SAFE_COORD_FOR_PED(position.x, position.y, position.z, true, &position, 16);
	return position;
}








Vehicle Misc::createVehicle(char* modelName, Vector3 pos, float heading)
{
	Hash model = MISC::GET_HASH_KEY(modelName);
	if (!STREAMING::HAS_MODEL_LOADED(model));
	{
		STREAMING::REQUEST_MODEL(model, false);
	}
	Vehicle wagon = VEHICLE::CREATE_VEHICLE(model, pos.x, pos.y, pos.z, heading, 1, 1, 0, 1);


	return wagon;
}

Ped Misc::createPedOnHorse(char* modelName, Ped horse, int seatIndex)
{
	Hash model = MISC::GET_HASH_KEY(modelName);
	STREAMING::REQUEST_MODEL(model, false);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Ped ped = PED::CREATE_PED_ON_MOUNT(horse, model, seatIndex, 0, 0, 0, 0);

	PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
	return ped;
}

Ped Misc::createPed(const char* modelName, Vector3 pos, float heading)
{
	Hash model = MISC::GET_HASH_KEY(modelName);
	STREAMING::REQUEST_MODEL(model, false);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Ped ped = PED::CREATE_PED(model, pos.x, pos.y, pos.z, heading, true, false, false, false);
	PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);

	return ped;
}

Vector3 Misc::toVector3(float x, float y, float z)
{
	Vector3 output;
	output.x = x;
	output.y = y;
	output.z = z;

	return output;
}



Object Misc::createProp(char* model, Vector3 position, float heading, bool isStatic, bool isVisible)
{
	Hash modelHash = MISC::GET_HASH_KEY(model);

	if (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		STREAMING::REQUEST_MODEL(modelHash, false);
	}

	while (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		WAIT(0);
	}

	Object prop = OBJECT::CREATE_OBJECT(modelHash, position.x, position.y, position.z, false, false, !isStatic, 0, 0);
	ENTITY::SET_ENTITY_HEADING(prop, heading);
	ENTITY::FREEZE_ENTITY_POSITION(prop, isStatic);
	ENTITY::SET_ENTITY_VISIBLE(prop, isVisible);

	return prop;
}

Object Misc::createProp(Hash model, Vector3 position, float heading, bool isStatic, bool isVisible)
{


	if (!STREAMING::HAS_MODEL_LOADED(model))
	{
		STREAMING::REQUEST_MODEL(model, false);
	}

	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}

	Object prop = OBJECT::CREATE_OBJECT(model, position.x, position.y, position.z, false, false, !isStatic, 0, 0);
	ENTITY::SET_ENTITY_HEADING(prop, heading);
	ENTITY::FREEZE_ENTITY_POSITION(prop, isStatic);
	ENTITY::SET_ENTITY_VISIBLE(prop, isVisible);

	return prop;
}


//void meong::mulai_atau_saya_culik();


void Misc::showSubtitle(const char* text)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE((char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text)); // _UILOG_SET_CACHED_OBJECTIVE
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE(); // _UILOG_PRINT_CACHED_OBJECTIVE
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE(); // _UILOG_CLEAR_CACHED_OBJECTIVE
}

void Misc::drawText(const char* text, float x, float y, int r, int g, int b, int a, bool centered, float sx, float sy)
{
	HUD::_SET_TEXT_COLOR(r, g, b, a);
	HUD::SET_TEXT_SCALE(sx, sy);
	HUD::SET_TEXT_CENTRE(centered);
	std::string s = text; ///////////////title
	std::string Template = "<FONT FACE='$title' COLOR='#%04X'>" + s + "</FONT>";
	const char* InsertTemplate = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", _strdup(Template.c_str()));
	HUD::_DISPLAY_TEXT(MISC::_CREATE_VAR_STRING(42, "COLOR_STRING", 0, InsertTemplate), x, y);
}

void Misc::drawTextSmall(const char* text, float x, float y, int r, int g, int b, int a, bool centered, float sx, float sy)
{
	HUD::_SET_TEXT_COLOR(r, g, b, a);
	HUD::SET_TEXT_SCALE(sx, sy);
	HUD::SET_TEXT_CENTRE(centered);
	std::string s = text;
	std::string Template = "<FONT FACE='$body' COLOR='#%04X'>" + s + "</FONT>";
	const char* InsertTemplate = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", _strdup(Template.c_str()));
	HUD::_DISPLAY_TEXT(MISC::_CREATE_VAR_STRING(42, "COLOR_STRING", 0, InsertTemplate), x, y);
}



void Misc::drawSprite(char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a)
{
	float fX = x + scalex / 2;
	float fY = y + scaley / 2;
	if (TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(category)) {
		GRAPHICS::DRAW_SPRITE(category, sprite, fX, fY, scalex, scaley, rotation, r, g, b, a, 0);
	}
	else {
		TXD::REQUEST_STREAMED_TEXTURE_DICT(category, 0);
	}




}




