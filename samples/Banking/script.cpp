#include "script.h"
#include <string>
#include <vector>

using namespace std;


const float menuX = 0.09f; //menuX 0.040
const float menuY = 0.02f; //menuY 0.68


Prompt PromptIn;
Prompt Prompt10;
Prompt Prompt50;
Prompt Prompt200;
Prompt PromptAll;

Prompt PromptBack;


Prompt PromptBanking;
Prompt PromptInvest;

Prompt PromptCredit;
Prompt PromptDeposit;

Prompt PromptWait;

Prompt PromptHorseBag;

Prompt CreditRate;
Prompt CreditIn;

Prompt Credit300;
Prompt Credit1000;
Prompt Credit5000;


Prompt Prompt_Donate_Up;
Prompt Prompt_Donate_Down;
Prompt Prompt_Donate;



void initialize()
{
	Misc::createPrompt(PromptBanking, "INPUT_SHOP_BUY", "Bank Services", 1, -2019190071);
	Misc::createPrompt(PromptCredit, "INPUT_JUMP", "Credits", 2, -2019190071);


	Misc::createPrompt(PromptDeposit, "INPUT_SPRINT", "Deposits", 2, -2019190071);

	Misc::createPrompt(PromptBack, "INPUT_FRONTEND_CANCEL", "Back", 2, -2019190071);



	Misc::createPrompt(PromptIn, "INPUT_INTERACT_OPTION2", "Cash-in all your money", 1, -2019190071);
	Misc::createPrompt(PromptAll, "INPUT_INTERACT_OPTION1", "Cash-out all your money", 1, -2019190071);

	Misc::createPrompt(Prompt10, "INPUT_SPRINT", "Cash-out $10", 2, -2019190071);
	Misc::createPrompt(Prompt50, "INPUT_JUMP", "Cash-out $50", 2, -2019190071);
	Misc::createPrompt(Prompt200, "INPUT_SHOP_BUY", "Cash-out $200", 2, -2019190071);


	Misc::createPrompt(PromptHorseBag, "INPUT_LOOK_BEHIND", "Take Saddlebag", 1);


	Misc::createPrompt(CreditIn, "INPUT_INTERACT_OPTION2", "Pay off your debt", 1, -2019190071);
	Misc::createPrompt(CreditRate, "INPUT_SPRINT", "Pay part of your debt", 0, -2019190071);



	Misc::createPrompt(Credit300, "INPUT_SPRINT", "$300 credit", 1, -2019190071);
	Misc::createPrompt(Credit1000, "INPUT_JUMP", "$1000 credit", 1, -2019190071);
	Misc::createPrompt(Credit5000, "INPUT_SHOP_BUY", "$5000 Credit", 1, -2019190071);


	Misc::createPrompt(Prompt_Donate_Up, "INPUT_INTERACT_OPTION1", "Increase Amount", 0);
	Misc::createPrompt(Prompt_Donate_Down, "INPUT_INTERACT_OPTION2", "Decrease Amount", 0);

	Misc::createPrompt(Prompt_Donate, "INPUT_FRONTEND_ACCEPT", "Cash-In", 0);


}




int bankMoney;
int bankDebt;

int sell_house = 0;

int debtType;

int debtDays;

void Save() {
	ofstream SaveFile;
	SaveFile.open("Banking.dat", std::ofstream::trunc);

	SaveFile << bankMoney << " " << bankDebt << " " << debtType << " " << debtDays;

	SaveFile.close();
}

void Load() {
	ifstream LoadFile;
	LoadFile.open("Banking.dat", std::ofstream::in);

	LoadFile >> bankMoney >> bankDebt >> debtType >> debtDays;

	LoadFile.close();
}


struct ScriptedSpeechParams
{
	const char* speechName;
	const char* voiceName;
	alignas(8) int v3;
	alignas(8) Hash speechParamHash;
	alignas(8) Entity entity;
	alignas(8) BOOL v6;
	alignas(8) int v7;
	alignas(8) int v8;
};


void main()
{		



	initialize();

	bool lose_money = GetPrivateProfileInt("BANKING", "LOSE_MONEY_ON_DEATH", 1, ".\\Banking.ini");
	float money_divide = GetPrivateProfileInt("BANKING", "PERCENT_OF_MONEY_LOST_ON_DEATH", 1, ".\\Banking.ini");

	int money_bag = GetPrivateProfileInt("BANKING", "MONEY_BAG_MECHANIC", 1, ".\\Banking.ini");
	int money_limit = GetPrivateProfileInt("BANKING", "MONEY_LIMIT", 500, ".\\Banking.ini");

	int armadillo_bank = GetPrivateProfileInt("MISC", "ARMADILLO_BANK", 1, ".\\Banking.ini");

	int sell_timer = 0;

	int banking_on = 0;

	int is_player_dead = 0;


	bool debt_on = 0;

	bool night_on = 0;


	int save_timer = 0;

	int night_timer = 0;

	int current_bank = 0; //0 - nothing; 1 - valentine; 2 - blackwater; 3 - rhodes; 4 - saint denis

	Vector3 local;

	int local_heading;

	Load();

	bool bagOn = 0;

	const int blipNumber = 5;

	Blip blips[blipNumber];

	Vector3 banks[blipNumber] = { Misc::toVector3(-307.649323, 776.651428, 118.762), Misc::toVector3(-812.822, -1277.791, 43.638), Misc::toVector3(1293.144, -1302.514, 77.041), Misc::toVector3(2643.956, -1292.622, 52.250), Misc::toVector3(-3665.463, -2625.200, -13.637) };



	Vector3 armadillo3 = Misc::toVector3(-3665.966, -2628.694, -13.588);
	Ped armadilloBanker = Misc::createPed("S_M_M_BankClerk_01", armadillo3, 8);
	TASK::_TASK_START_SCENARIO_IN_PLACE(armadilloBanker, MISC::GET_HASH_KEY("WORLD_HUMAN_VAL_BANKTELLER"), -1, true, false, 0, 0);

	bool inBank = 0;

	int hold_clock = 0;

	bool day_passed = 0;

	int cop_timer = MISC::GET_GAME_TIMER() + 4000 * 60;;

	while (true)
	{
		Ped player = PLAYER::PLAYER_PED_ID();
		Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(player, true, false);
		
		
		Ped playerHorse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(0);
		Vector3 horsePos = ENTITY::GET_ENTITY_COORDS(playerHorse, true, false);


		HUD::_UIPROMPT_SET_VISIBLE(PromptWait, 0);
		HUD::_UIPROMPT_SET_ENABLED(PromptWait, 0);



		HUD::_UIPROMPT_SET_VISIBLE(Prompt50, 0);
		HUD::_UIPROMPT_SET_ENABLED(Prompt50, 0);

		HUD::_UIPROMPT_SET_VISIBLE(Prompt10, 0);
		HUD::_UIPROMPT_SET_ENABLED(Prompt10, 0);

		HUD::_UIPROMPT_SET_VISIBLE(PromptAll, 0);
		HUD::_UIPROMPT_SET_ENABLED(PromptAll, 0);

		HUD::_UIPROMPT_SET_VISIBLE(Prompt200, 0);
		HUD::_UIPROMPT_SET_ENABLED(Prompt200, 0);

		HUD::_UIPROMPT_SET_VISIBLE(PromptIn, 0);
		HUD::_UIPROMPT_SET_ENABLED(PromptIn, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptBack, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptBack, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptBanking, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptBanking, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptInvest, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptInvest, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptCredit, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptCredit, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptDeposit, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptDeposit, 0);

		HUD::_UIPROMPT_SET_ENABLED(Credit300, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Credit300, 0);

		HUD::_UIPROMPT_SET_ENABLED(Credit1000, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Credit1000, 0);

		HUD::_UIPROMPT_SET_ENABLED(Credit5000, 0);
		HUD::_UIPROMPT_SET_VISIBLE(Credit5000, 0);

		HUD::_UIPROMPT_SET_ENABLED(PromptHorseBag, 0);
		HUD::_UIPROMPT_SET_VISIBLE(PromptHorseBag, 0);


		HUD::_UIPROMPT_SET_ENABLED(CreditIn, 0);
		HUD::_UIPROMPT_SET_VISIBLE(CreditIn, 0);

		HUD::_UIPROMPT_SET_ENABLED(CreditRate, 0);
		HUD::_UIPROMPT_SET_VISIBLE(CreditRate, 0);


		if (!MISC::GET_MISSION_FLAG()) {
			if (cop_timer < MISC::GET_GAME_TIMER() && debtDays <= 0 && bankDebt > 0) {
				LAW::_SET_BOUNTY_FOR_PLAYER(0, bankDebt * 100);
				LAW::_0x956510F8C36B5C64();
				cop_timer = MISC::GET_GAME_TIMER() + 40000 * 60;
			}
		}
		save_timer++;
		if (save_timer > 600) {
			Save();

			save_timer = 0;
		}

		std::stringstream budget;
		budget << "$";
		budget << bankMoney / 100;

		std::stringstream debtsum;
		debtsum << "$";
		debtsum << bankDebt;


		if (money_bag) {
			Entity entHorse;
			if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(0) && distanceBetween(playerPos, horsePos) <= 2.7f) {
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(0, &entHorse)) {
					if (ENTITY::IS_ENTITY_A_PED(entHorse)) {
						Ped targetPed = entHorse;


						if (targetPed == playerHorse) {

							HUD::_UIPROMPT_SET_ENABLED(PromptHorseBag, 1);
							HUD::_UIPROMPT_SET_VISIBLE(PromptHorseBag, 1);
							HUD::_UIPROMPT_SET_GROUP(PromptHorseBag, HUD::_UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(entHorse), 0);



						}

					}
				}
			}
		}

		hold_clock++;
		if (hold_clock > 80) {
			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(PromptHorseBag)) {


				if (!bagOn && !PED::_IS_PED_COMPONENT_EQUIPPED(player, 0x79D7DF96)) {
					PED::_SET_PED_COMPONENT_ENABLED(player, 0x1C92E7D8, 1, 0, 0);
					PED::_UPDATE_PED_VARIATION(player, 1, 1, 1, 1, 1);


					TASK::TASK_ANIMAL_INTERACTION(player, playerHorse, MISC::GET_HASH_KEY("Interaction_LootSaddleBags"), 0, 1);



					PED::_SET_PED_COMPONENT_DISABLED(playerHorse, 0x80451C25, 1);
					PED::_UPDATE_PED_VARIATION(playerHorse, 1, 1, 1, 1, 1);
				}





				if (bagOn && PED::_IS_PED_COMPONENT_EQUIPPED(player, 0x79D7DF96)) {
					PED::_SET_PED_COMPONENT_DISABLED(player, 0x79D7DF96, 1);
					PED::_UPDATE_PED_VARIATION(player, 1, 1, 1, 1, 1);


					TASK::TASK_ANIMAL_INTERACTION(player, playerHorse, MISC::GET_HASH_KEY("Interaction_LootSaddleBags"), 0, 1);


					PED::_SET_PED_COMPONENT_DISABLED(playerHorse, 0x80451C25, 1);
					PED::_SET_PED_COMPONENT_ENABLED(playerHorse, MISC::GET_HASH_KEY("HORSE_EQUIPMENT_HORSE_SADDLEBAGUPGRADE_NEW_004"), 1, 0, 1);
					PED::_UPDATE_PED_VARIATION(playerHorse, 1, 1, 1, 1, 1);
				}

				hold_clock = 0;
			}
		}


		if (!bagOn && money_bag) {
			if (MONEY::_MONEY_GET_CASH_BALANCE() > (money_limit * 100)) {
				MONEY::_MONEY_DECREMENT_CASH_BALANCE(99999999);
				MONEY::_MONEY_INCREMENT_CASH_BALANCE(money_limit * 100, 0);
			}
		}

		if (PED::_IS_PED_COMPONENT_EQUIPPED(player, 0x79D7DF96)) {
			bagOn = 1;

			const char* textLabel = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Place Saddlebag"); 


			HUD::_UIPROMPT_SET_TEXT(PromptHorseBag, (char*)textLabel); 

		}
		else {
			bagOn = 0;
			const char* textLabel = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Take Saddlebag"); 


			HUD::_UIPROMPT_SET_TEXT(PromptHorseBag, (char*)textLabel); 

		}

		Entity ent;
		if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(0)) {
			if (PLAYER::GET_PLAYER_TARGET_ENTITY(0, &ent)) {
				if (ENTITY::IS_ENTITY_A_PED(ent)) {
					Ped targetPed = ent;


					if (PED::IS_PED_MODEL(targetPed, MISC::GET_HASH_KEY("S_M_M_BankClerk_01"))) {

						PED::SET_PED_CONFIG_FLAG(targetPed, 130, 1);


						HUD::_UIPROMPT_SET_ENABLED(PromptBanking, 1); // _UIPROMPT_SET_ENABLED
						HUD::_UIPROMPT_SET_VISIBLE(PromptBanking, 1); // _UIPROMPT_SET_VISIBLE
						HUD::_UIPROMPT_SET_GROUP(PromptBanking, HUD::_UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(ent), 0);



					}

				}
			}
		}

		if (banking_on > 0) {
			HUD::_UIPROMPT_SET_ACTIVE_GROUP_THIS_FRAME(-2019190071, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Bank Services"), 1, 0, 0, 0);
		}

		if (banking_on == 1) {


			HUD::_UIPROMPT_SET_ENABLED(PromptCredit, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptCredit, 1);


			HUD::_UIPROMPT_SET_ENABLED(PromptDeposit, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptDeposit, 1);

			HUD::_UIPROMPT_SET_ENABLED(PromptBack, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptBack, 1);


		}




		if (HUD::_UIPROMPT_IS_JUST_RELEASED(PromptDeposit)) {
			banking_on = 2;
		}


		if (HUD::_UIPROMPT_IS_JUST_RELEASED(PromptCredit)) {
			banking_on = 3;
		}



		if (banking_on == 2) {


			HUD::_UIPROMPT_SET_ENABLED(PromptAll, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptAll, 1);

			HUD::_UIPROMPT_SET_ENABLED(PromptIn, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptIn, 1);

			HUD::_UIPROMPT_SET_ENABLED(Prompt10, 1);
			HUD::_UIPROMPT_SET_VISIBLE(Prompt10, 1);

			HUD::_UIPROMPT_SET_ENABLED(Prompt50, 1);
			HUD::_UIPROMPT_SET_VISIBLE(Prompt50, 1);

			HUD::_UIPROMPT_SET_ENABLED(Prompt200, 1);
			HUD::_UIPROMPT_SET_VISIBLE(Prompt200, 1);

			HUD::_UIPROMPT_SET_ENABLED(PromptBack, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptBack, 1);

		}

		if (banking_on == 3) {

			
				
				if (bankDebt > 0) {

					if (bankMoney >= bankDebt) {
						HUD::_UIPROMPT_SET_ENABLED(CreditIn, 1);
					}

					switch (debtType) {
					case 1:
						HUD::_UIPROMPT_SET_TEXT(CreditRate, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Pay part of the debt ($50)"));
						
						
						
						if (debtDays != 7) {
							if (bankMoney >= 50 * 100) {
								HUD::_UIPROMPT_SET_ENABLED(CreditRate, 1);
							}
						}

						

						break;
					case 2:
						HUD::_UIPROMPT_SET_TEXT(CreditRate, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Pay part of the debt ($200)"));
						if (debtDays != 7) {
							if (bankMoney >= 200 * 100) {
								HUD::_UIPROMPT_SET_ENABLED(CreditRate, 1);
							}
						}


						break;
					case 3:
						HUD::_UIPROMPT_SET_TEXT(CreditRate, (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Pay part of the debt ($500)"));
						if (debtDays != 7) {
							if (bankMoney >= 500 * 100) {
								HUD::_UIPROMPT_SET_ENABLED(CreditRate, 1);
							}
						}


						break;
					}


					
				
					HUD::_UIPROMPT_SET_VISIBLE(CreditIn, 1);

					
					HUD::_UIPROMPT_SET_VISIBLE(CreditRate, 1);


				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(Credit300, 1);
					HUD::_UIPROMPT_SET_VISIBLE(Credit300, 1);

					HUD::_UIPROMPT_SET_ENABLED(Credit1000, 1);
					HUD::_UIPROMPT_SET_VISIBLE(Credit1000, 1);

					HUD::_UIPROMPT_SET_ENABLED(Credit5000, 1);
					HUD::_UIPROMPT_SET_VISIBLE(Credit5000, 1);

				}

			


			HUD::_UIPROMPT_SET_ENABLED(PromptBack, 1);
			HUD::_UIPROMPT_SET_VISIBLE(PromptBack, 1);

		}




		if (distanceBetween(playerPos, banks[0]) <= 4.3f) {
			current_bank = 1;
			local = Misc::toVector3(-308.599, 775.953, 118.702);
			local_heading = 198;
		}
		if (distanceBetween(playerPos, banks[1]) <= 4.f) {
			current_bank = 3;
			local = Misc::toVector3(-813.2, -1277.490, 43.638);
			local_heading = 1;
		}
		if (distanceBetween(playerPos, banks[2]) <= 6.f) {
			current_bank = 2;
			local = Misc::toVector3(1294.182, -1303.100, 77.042);
			local_heading = 143;
		}
		if (distanceBetween(playerPos, banks[3]) <= 6.5f) {
			current_bank = 4;
			local = Misc::toVector3(2644.171, -1292.583, 52.249);
			local_heading = 199;
		}
		if (distanceBetween(playerPos, banks[4]) <= 6.5f) {
			current_bank = 5;
			local = Misc::toVector3(-3666.098, -2626.563, -13.588);
			local_heading = 180;
		}



		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(PromptBanking)) {

			TASK::TASK_GO_TO_COORD_ANY_MEANS(player, local.x, local.y, local.z, 0.3f, 0, 0, 0, 0);

			banking_on = 10;

		}




		if (banking_on == 10) {



			if (distanceBetween(playerPos, local) <= 0.7) {
				//TASK::TASK_PED_SLIDE_TO_COORD(player, playerPos.x, playerPos.y, playerPos.z, 198, 0);

				//TASK::CLEAR_PED_TASKS(player, 1, 1);

				//WAIT(0);

				//Object seq;
				//TASK::OPEN_SEQUENCE_TASK(&seq);
				//AI::TASK_TURN_PED_TO_FACE_COORD(0, posterCoords.x, posterCoords.y, posterCoords.z, 1000);


				inBank = 1;
				TASK::TASK_START_SCENARIO_AT_POSITION(player, MISC::GET_HASH_KEY("WORLD_HUMAN_VAL_BANKTELLER"), local.x, local.y, local.z, local_heading, -1, true, false, 0, 0, false);




				//TASK::CLOSE_SEQUENCE_TASK(seq);
				//TASK::CLEAR_PED_TASKS(player, 1, 1);
				//TASK::TASK_PERFORM_SEQUENCE(player, seq);

				banking_on = 1;
			}


		}


		if (HUD::_UIPROMPT_IS_JUST_RELEASED(PromptBack)) {

			if (banking_on == 1) {
				TASK::CLEAR_PED_TASKS(player, 1, 1);
				banking_on = 0;
			}

			if (banking_on > 1) {
				banking_on = 1;
			}


		}

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(PromptInvest)) {

		}


		int money_current = MONEY::_MONEY_GET_CASH_BALANCE();

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(PromptIn))
		{
			;

			bankMoney += money_current;



			MONEY::_MONEY_DECREMENT_CASH_BALANCE(99999999999);




		}

		if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt10)) { //_HUDPROMPT_IS_JUST_RELEASED


			if (bankMoney >= 1000) {
				MONEY::_MONEY_INCREMENT_CASH_BALANCE(1000, 1);
				bankMoney -= 1000;
			}


		}

		if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt50)) { //_HUDPROMPT_IS_JUST_RELEASED

			if (bankMoney >= 5000) {
				MONEY::_MONEY_INCREMENT_CASH_BALANCE(5000, 1);
				bankMoney -= 5000;
			}

		}

		if (HUD::_UIPROMPT_IS_JUST_RELEASED(Prompt200)) { //_HUDPROMPT_IS_JUST_RELEASED


			if (bankMoney >= 20000) {
				MONEY::_MONEY_INCREMENT_CASH_BALANCE(20000, 1);
				bankMoney -= 20000;
			}

		}

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(PromptAll)) { //_UIPROMPT_IS_JUST_RELEASED

			MONEY::_MONEY_INCREMENT_CASH_BALANCE(bankMoney, 1);
			bankMoney = 0;

		}


		sell_timer++;
		if (sell_house > 0 && sell_timer > 1000) {
			sell_house = 0;
			sell_timer = 0;
		}



		if (bankDebt == 0) {


			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Credit300)) { 

				AUDIO::_0x0F2A2175734926D8("PURCHASE", "Ledger_Sounds");

				AUDIO::PLAY_SOUND_FRONTEND("PURCHASE", "Ledger_Sounds", 1, 0);


				bankDebt = 300;
				
				debtType = 1;

				

				bankMoney += 30000;

			}

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Credit1000)) {

				AUDIO::_0x0F2A2175734926D8("PURCHASE", "Ledger_Sounds");

				AUDIO::PLAY_SOUND_FRONTEND("PURCHASE", "Ledger_Sounds", 1, 0);


				bankDebt = 1000;
				
				debtType = 2;

				bankMoney += 100000;

			}

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Credit5000)) {

				AUDIO::_0x0F2A2175734926D8("PURCHASE", "Ledger_Sounds");

				AUDIO::PLAY_SOUND_FRONTEND("PURCHASE", "Ledger_Sounds", 1, 0);


				bankDebt = 5000;
				
				debtType = 3;

				bankMoney += 500000;

			}



			
		}
		else {

			if (CLOCK::GET_CLOCK_HOURS() == 0) {
				if (day_passed == 0) {
					debtDays--;

					day_passed = 1;
				}
	
			}

			if (CLOCK::GET_CLOCK_HOURS() == 1) {
				day_passed = 0;
			}


			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(CreditIn)) {

				AUDIO::_0x0F2A2175734926D8("PURCHASE", "Ledger_Sounds");

				AUDIO::PLAY_SOUND_FRONTEND("PURCHASE", "Ledger_Sounds", 1, 0);

				bankMoney -= (bankDebt * 100);

				bankDebt = 0;
				
			}

			if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(CreditRate)) {
				
				AUDIO::_0x0F2A2175734926D8("PURCHASE", "Ledger_Sounds");

				AUDIO::PLAY_SOUND_FRONTEND("PURCHASE", "Ledger_Sounds", 1, 0);

				switch (debtType) {
				case 1:
					bankMoney -= 25 * 100;
					bankDebt -= 25;
					break;
				case 2:
					bankMoney -= 100 * 100;
					bankDebt -= 100;
					break;
				case 3:
					bankMoney -= 250 * 100;
					bankDebt -= 250;
					break;
				}

				if (bankDebt > 0) {
					debtDays = 7;
				}
				
			}

		}


		if (debtDays == 0 && bankDebt > 0) {

			switch (debtType) {
			case 1:

				if (bankMoney >= 50 * 100) {
					bankMoney -= 50 * 100;
					bankDebt -= 50;
				}
				
				
				break;
			case 2:

				if (bankMoney >= 200 * 100) {
					bankMoney -= 200 * 100;
					bankDebt -= 200;
				}
				break;
			case 3:
				if (bankMoney >= 500 * 100) {
					bankMoney -= 500 * 100;
					bankDebt -= 500;
				}
				break;
			}
		}
		

		if (bankDebt < 0) {
			bankDebt = 0;
		}


		if (!INTERIOR::IS_INTERIOR_SCENE() && inBank)
		{
			inBank = 0;
			ScriptedSpeechParams params{ "PLAYER_SHOP_THANKS_NEUTRAL", 0, -1, 0x67F3AB43, -1, true, -1, -1 };
			AUDIO::_PLAY_AMBIENT_SPEECH1(player, (Any*)&params);

		}


		OBJECT::_0xD99229FE93B46286(3101287960, 1, 1, 1, 1, 1, 1);

		if (distanceBetween(playerPos, banks[0]) <= 4.3f || distanceBetween(playerPos, banks[1]) <= 4.f || distanceBetween(playerPos, banks[2]) <= 6.f || distanceBetween(playerPos, banks[3]) <= 6.5f || distanceBetween(playerPos, banks[4]) <= 4.5f)
		{

			HUD::_0xC9CAEAEEC1256E54(MISC::GET_HASH_KEY("HUD_CTX_IN_SHOP"));

			Misc::drawText(budget.str().c_str(), menuX + 0.8452f, menuY + 0.047f, 0, 0, 0, 255, 1, 0.6f, 0.6f);
			Misc::drawText(budget.str().c_str(), menuX + 0.845f, menuY + 0.045f, 255, 255, 255, 255, 1, 0.6f, 0.6f);
			Misc::drawSprite("hud_textures", "camp_tent", menuX + 0.037 + 0.833, menuY + 0.045f, 0.018f, 0.033f, 0, 255, 255, 255, 255);


			Misc::drawText(debtsum.str().c_str(), menuX + 0.8452f, menuY + 0.047f + 0.04f, 0, 0, 0, 255, 1, 0.6f, 0.6f);
			Misc::drawText(debtsum.str().c_str(), menuX + 0.845f, menuY + 0.045f + 0.04f, 255, 255, 255, 255, 1, 0.6f, 0.6f);
			Misc::drawSprite("hud_textures", "gang_savings", menuX + 0.037 + 0.833, menuY + 0.045f + 0.04f, 0.018f, 0.033f, 0, 255, 255, 255, 255);






			if (banking_on == 2) {
				if (bankMoney >= 10 * 100) {
					HUD::_UIPROMPT_SET_ENABLED(Prompt10, 1);
				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(Prompt10, 0);
				}






				if (bankMoney >= 50 * 100) {
					HUD::_UIPROMPT_SET_ENABLED(Prompt50, 1);
				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(Prompt50, 0);
				}



				if (bankMoney >= 200 * 100) {

					HUD::_UIPROMPT_SET_ENABLED(Prompt200, 1);
				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(Prompt200, 0);
				}




				if (bankMoney > 0) {

					HUD::_UIPROMPT_SET_ENABLED(PromptAll, 1); //is enabled
				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(PromptAll, 0);
				}




				if (MONEY::_MONEY_GET_CASH_BALANCE() > 0) {
					HUD::_UIPROMPT_SET_ENABLED(PromptIn, 1);
				}
				else {
					HUD::_UIPROMPT_SET_ENABLED(PromptIn, 0);
				}
			}





			if (armadillo_bank) {
				for (int i = 0; i < blipNumber; i++) {
					if (MAP::DOES_BLIP_EXIST(blips[i])) {
						MAP::REMOVE_BLIP(&blips[i]);
					}
				}
			}
			else {
				for (int i = 0; i < blipNumber - 1; i++) {
					if (MAP::DOES_BLIP_EXIST(blips[i])) {
						MAP::REMOVE_BLIP(&blips[i]);
					}
				}
			}





		}
		else {
			current_bank = 0;


			if (CLOCK::GET_CLOCK_HOURS() > 23 || CLOCK::GET_CLOCK_HOURS() < 6) {

				night_timer++;
				if (night_timer > 200) {
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2642457609, 1); //Valentine bank doors
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3886827663, 1);

					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(1733501235, 1); //Saint Denis bank
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2089945615, 1);
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2158285782, 1);
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2817024187, 1);

					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3317756151, 1); //Rhodes bank
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3088209306, 1);

					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(531022111, 1); //Blackwater bank


					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3101287960, 1); //armadillo


				}

				if (armadillo_bank) {
					for (int i = 0; i < blipNumber; i++) {
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_TOD_DAYTIME_ONLY"));
					}
				}
				else {
					for (int i = 0; i < blipNumber - 1; i++) {
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_TOD_DAYTIME_ONLY"));
					}
				}



				night_on = 1;
			}
			else {
				night_timer = 0;

				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2642457609, 0); //Valentine bank doors
				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3886827663, 0);

				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(1733501235, 0); //Saint Denis bank
				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2089945615, 0);
				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2158285782, 0);
				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(2817024187, 0);

				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3317756151, 0); //Rhodes bank
				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3088209306, 0);

				OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(531022111, 0); //Blackwater bank

				if (armadillo_bank) {
					OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(3101287960, 0); //armadillo
				}

				if (night_on) {
					if (armadillo_bank) {
						for (int i = 0; i < blipNumber; i++) {
							if (MAP::DOES_BLIP_EXIST(blips[i])) {
								MAP::REMOVE_BLIP(&blips[i]);
							}
						}
					}
					else {
						for (int i = 0; i < blipNumber - 1; i++) {
							if (MAP::DOES_BLIP_EXIST(blips[i])) {
								MAP::REMOVE_BLIP(&blips[i]);
							}
						}
					}



					night_on = 0;
				}
			}

			if (armadillo_bank) {
				for (int i = 0; i < blipNumber; i++) {
					if (!MAP::DOES_BLIP_EXIST(blips[i])) {
						blips[i] = MAP::_BLIP_ADD_FOR_COORD(0x63351D54, banks[i].x, banks[i].y, banks[i].z);
						MAP::SET_BLIP_SPRITE(blips[i], MISC::GET_HASH_KEY("blip_proc_bank"), false); //blip_proc_bank 0x25249A47
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_RADAR_EDGE_NEVER"));
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_HIDE_HEIGHT_MARKER"));

						MAP::_SET_BLIP_NAME_FROM_PLAYER_STRING(blips[i], (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Bank"));
					}
				}
			}
			else {
				for (int i = 0; i < blipNumber - 1; i++) {
					if (!MAP::DOES_BLIP_EXIST(blips[i])) {
						blips[i] = MAP::_BLIP_ADD_FOR_COORD(0x63351D54, banks[i].x, banks[i].y, banks[i].z);
						MAP::SET_BLIP_SPRITE(blips[i], MISC::GET_HASH_KEY("blip_proc_bank"), false); //blip_proc_bank 0x25249A47
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_RADAR_EDGE_NEVER"));
						MAP::_BLIP_SET_MODIFIER(blips[i], MISC::GET_HASH_KEY("BLIP_MODIFIER_HIDE_HEIGHT_MARKER"));

						MAP::_SET_BLIP_NAME_FROM_PLAYER_STRING(blips[i], (char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Bank"));
					}
				}
			}

		}






		if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()) == true) {
			if (is_player_dead == 0) {
				is_player_dead = 1;

				if (lose_money == 1) {

					MONEY::_MONEY_DECREMENT_CASH_BALANCE(MONEY::_MONEY_GET_CASH_BALANCE() * (money_divide / 100));
				}

			}

		}
		else {
			is_player_dead = 0;
		}






		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
