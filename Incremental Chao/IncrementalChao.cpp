#include "pch.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "SA2programming\UsercallFunctionHandler.h"//to remember: check mods for compatibile
#include "shadd.h"

#ifdef _DEBUG
#define DEBUGX(x) x;
#else
#define DEBUGX(x)
#endif

extern "C" {
	double verifyConfigMult(std::string);
	void doNothing();
	
	//debug
	std::ostringstream logstream;
	bool dummy = false;
	int debugFrame = 0;
	int debugInterval = 75;

	//config options
	double ringMult = 1.0, statMult = 1.0, xpMult = 1.0, animalMult = 1.0, driveMult = 1.0;
	bool enableAllGardens = false;

	//Mod Balance/functionality vars
	int ringFrame = 0;
	int ringInterval = 360;//60 = 1 second
	double ringBase = 60.0;
	double ringLeftover = 0.0;
	SHADD_AL_TIME* rtime = &RaceInfo.time;//timer!

	__int16 gardenStatMax[2][7];//The the grade of the stat acts as the index value.  [0][x] for standard, [1][x] for run (x grade CWE)
	__int16 gardenStatMaxLI[9] = { 0 };//Luck, Intelligence can't pass 4000 so it needs a few more indexes.  [8] hits the hard cap.

	int changeChaoFrame = 0;
	int changeChaoInterval = 240;//60 = 1 second, minimum working value 16
	double statBase = 432000;//60 fps * 60s/min * 60min/h * 2h = 432000 frame
	double statLeftovers[24][8] = { 0 };

	//persistent data vars
	char gardenStatLevels[8] = { 0 };
	int gardenStatFractions[8] = { 0 };
	
	bool gardenHasBeginnerMedal = false;
	bool loadedgardenIncrementData = false;

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Executed at startup, contains helperFunctions and the path to your mod (useful for getting the config file.)
		// This is where we override functions, replace static data, etc.
		
		DEBUGX(logstream << "Incremental Chao: Init config path: " << path << '\n';);

		//init statMaxes (based on grades)
		{
			for (int i = 0; i < 6; i++) {
				gardenStatMax[0][i] = 99 * (15 + 3 * i);
				for (int j = 0; j < 5; j++) {
					gardenStatMax[0][i] = gardenStatMax[0][i] / 10 + 98 * (15 + 3 * i);
				}
				gardenStatMax[1][i] = gardenStatMax[0][i];
				gardenStatMaxLI[i] = gardenStatMax[0][i];
				DEBUGX(logstream << "Incremental Chao: gardenStatMaxLI[" << i << "]: " << gardenStatMaxLI[i] << "\n";);
			}
			//CWE x grade compatibility
			gardenStatMax[0][6] = 3854;//formula should be 3919.  Modified for realistic acquisition? (gain level when eating fruit at s or something)
			gardenStatMax[1][6] = 3560;//formula should be 3593
			for (int i = 6; i < 8; i++) {
				gardenStatMaxLI[i] = 99 * (15 + 3 * i);
				for (int j = 0; j < 5; j++) {
					gardenStatMaxLI[i] = gardenStatMaxLI[i] / 10 + 98 * (15 + 3 * i);
				}
				DEBUGX(logstream << "Incremental Chao: gardenStatMaxLI[" << i << "]: " << gardenStatMaxLI[i] << "\n";);
			}
			gardenStatMaxLI[8] = 4000;//4000 is the cap for luck and intelligence
		}

		//config file
		std::ifstream config;
		std::string configPath = path;
		configPath += "\\config.ini";
		DEBUGX(logstream << "Incremental Chao: " << configPath << '\n';);
		std::string configLine;
		config.open(configPath);
		if (config.is_open()) {//set up config vars
			while (getline(config, configLine)) {
				//DEBUGX(logstream << "configLine: " << configLine << '\n';);
				int configEqualPos = configLine.find("=");
				if (configEqualPos != std::string::npos) {
					std::string lineName = configLine.substr(0, configEqualPos);
					std::string lineValue = configLine.substr(configEqualPos + 1, std::string::npos);
					//DEBUGX(logstream << "lineName: " << lineName << "\nlineValue: " << lineValue << '\n';);

					if (lineName == "ringMultiplier") {
						ringMult = verifyConfigMult(lineValue);
					}
					else if (lineName == "statMultiplier") {
						statMult = verifyConfigMult(lineValue);
					}
					else if (lineName == "experienceMultiplier") {
						xpMult = verifyConfigMult(lineValue);
					}
					else if (lineName == "animalMultiplier") {
						animalMult = verifyConfigMult(lineValue);
					}
					else if (lineName == "driveMultiplier") {
						driveMult = verifyConfigMult(lineValue);
					}
					else if (lineName == "enableAllGardens") {
						if (lineValue == "True") {
							enableAllGardens = true;
						}
						else if (lineValue == "False") {
							enableAllGardens = false;
						}
					}
				}
			}
			config.close();
		}
		else {
			logstream << "Incremental Chao: config file not open\n";
		}

		logstream << "Incremental Chao: ringMult: " << ringMult << '\n';
		logstream << "Incremental Chao: statMult: " << statMult << '\n';
		logstream << "Incremental Chao: xpMult: " << xpMult << '\n';
		logstream << "Incremental Chao: animalMult: " << animalMult << '\n';
		logstream << "Incremental Chao: driveMult: " << driveMult << '\n';
		logstream << "Incremental Chao: enableAllGardens: " << enableAllGardens << '\n';
		
		if (!logstream.str().empty()) {
			PrintDebug(logstream.str().c_str());
			logstream.str("");
		}
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		// Executed every running frame of SA2

		const int chaoAreaID = GetCurrentChaoStage();
		//DEBUGX(logstream << "Incremental Chao: chaoAreaID: " << chaoAreaID << "\n";);

		if (chaoWorldIsLoaded && !loadedgardenIncrementData) {//Entering chao world, get data (HACKY, TO BE FIXED?) (init, part 1)
			for (int iterChaoSlots = 0; iterChaoSlots < 24; ++iterChaoSlots) {
				//ChaoDataBase& ccd = ChaoSlots[iterChaoSlots].data;
				for (int iterStatLevels = 0; iterStatLevels < 8; ++iterStatLevels) {//init stats
					if (ChaoSlots[iterChaoSlots].data.StatLevels[iterStatLevels] > gardenStatLevels[iterStatLevels]) {
						gardenStatLevels[iterStatLevels] = ChaoSlots[iterChaoSlots].data.StatLevels[iterStatLevels];//set garden level
						gardenStatFractions[iterStatLevels] = 100 * gardenStatLevels[iterStatLevels] + ChaoSlots[iterStatLevels].data.StatPoints[7]; //fraction includes saved
					}
				}
				if (!gardenHasBeginnerMedal && ChaoSlots[iterChaoSlots].data.DoctorMedal & 2) gardenHasBeginnerMedal = true;//init medal
			}
			DEBUGX(
				for (int i = 0; i < 8; i++) {
					logstream << "Incremental Chao: " << i << ":" << gardenStatFractions[i] << "\n";
				}
			);
			loadedgardenIncrementData = true;
			logstream << "Incremental Chao: garden loaded.\n";
		}
		else if (!chaoWorldIsLoaded && loadedgardenIncrementData) {
			for (int iterStatLevels = 0; iterStatLevels < 8; ++iterStatLevels) {//unload persistent data
				gardenStatLevels[iterStatLevels] = 0;
				gardenStatFractions[iterStatLevels] = 0;
				gardenHasBeginnerMedal = false;
			}
			loadedgardenIncrementData = false;
			logstream << "Incremental Chao: garden unloaded.\n";
		}

		if (chaoWorldIsLoaded && GameState == GameStates_Ingame) {
			
			if (ringFrame == ringInterval - 1) {//ring loop
				bool isRacing = (chaoAreaID == 4 && (rtime->frame || rtime->second || rtime->minute));
				//DEBUGX(logstream << "Incremental Chao: rtime: " << static_cast<int>(rtime->frame) << ":" << static_cast<int>(rtime->second) << ":" << static_cast<int>(rtime->minute) << "\n";);
				
				//if rings are not maxed, increment them to threshold
				if (TotalRings < 9999999) {
					double preciseRings = ringInterval / ringBase * (1.0 + static_cast<double>(isRacing)) 
						* (1.0 + static_cast<double>(gardenHasBeginnerMedal)) * ringMult + ringLeftover;
					int incrementRings = static_cast<int>(preciseRings);
					if (TotalRings + incrementRings < 9999999) {
						TotalRings += incrementRings;
						ringLeftover = preciseRings - static_cast<double>(incrementRings);
					}
					else {
						TotalRings = 9999999;
						ringLeftover = 0.0;
					}
				}
			}
			ringFrame += 1;
			ringFrame = ringFrame % ringInterval;
			
			//detect changes on every odd sixteenth of an interval, increment chao on every even sixteenth of an interval
			bool detectConditionFrame = detectConditionFrame = fmod(static_cast<double>(changeChaoFrame + 1), (static_cast<double>(changeChaoInterval) / 16.0)) < 1.0;
			bool incrementConditionFrame = fmod(static_cast<double>(changeChaoFrame + 1), (static_cast<double>(changeChaoInterval) / 8.0)) < 1.0;
			
			if (chaoAreaID <= 4 && 1 <= chaoAreaID) {
				if (detectConditionFrame && !incrementConditionFrame) {
					DEBUGX(logstream << "Incremental Chao: detectConditionFrame: " << changeChaoFrame << "\n";);

					//Part 2 HACKY JANKY
					//detect changes to chao status and forward to garden data (handle animals and chaos drives)
					for (int iterChaoSlots = 0; iterChaoSlots < 24; ++iterChaoSlots) {
						ChaoDataBase* ccdp = &ChaoSlots[iterChaoSlots].data;//ccdp = current chao data pointer
						if (!gardenHasBeginnerMedal && (*ccdp).DoctorMedal & 2) gardenHasBeginnerMedal = true;//detect medal

						//init fractions on chao
						char* ccdfracAddrs[7];
						ccdfracAddrs[0] = &(*ccdp).SwimFraction;
						ccdfracAddrs[1] = &(*ccdp).FlyFraction;
						ccdfracAddrs[2] = &(*ccdp).RunFraction;
						ccdfracAddrs[3] = &(*ccdp).PowerFraction;
						ccdfracAddrs[4] = &(*ccdp).StaminaFraction;
						ccdfracAddrs[5] = &(*ccdp).LuckyFraction;
						ccdfracAddrs[6] = &(*ccdp).IntelligenceFraction;

						//find and forward changes to stat fractions and levels
						for (int iterChaoStats = 0; iterChaoStats < 7; ++iterChaoStats) {//loop stat type
							//check to change garden level if chao are modified in editor
							if ((*ccdp).StatLevels[iterChaoStats] > gardenStatLevels[iterChaoStats]) {
								gardenStatLevels[iterChaoStats] = (*ccdp).StatLevels[iterChaoStats];
								gardenStatFractions[iterChaoStats] = static_cast<int>(gardenStatLevels[iterChaoStats]) * 100;
							}
							int addVal = static_cast<int>(xpMult * static_cast<double>(*ccdfracAddrs[iterChaoStats]));

							gardenStatFractions[iterChaoStats] = gardenStatFractions[iterChaoStats] + addVal > 9999
								? 9999 : gardenStatFractions[iterChaoStats] + addVal;//add fraction to garden stats

							ChaoSlots[iterChaoStats].data.StatPoints[7] = static_cast<__int16>(gardenStatFractions[iterChaoStats] % 100);//set fraction to save
							*ccdfracAddrs[iterChaoStats] = 0;//set chao fraction to 0
							gardenStatLevels[iterChaoStats] = gardenStatFractions[iterChaoStats] / 100;//correct garden level
							(*ccdp).StatLevels[iterChaoStats] = gardenStatLevels[iterChaoStats];//update chao level
						}

					}
					//DEBUGX(logstream << "Incremental Chao: doctormedal on chao0: " << static_cast<int>(ChaoSlots[0].data.DoctorMedal) << "\n";);
					//DEBUGX(logstream << "Incremental Chao: beginnermedal: " << static_cast<int>(gardenHasBeginnerMedal) << "\n";);
				}
			}
			if (chaoAreaID <= 3 && 1 <= chaoAreaID) {//chao loop only in gardens
				if (incrementConditionFrame) {
					DEBUGX(logstream << "Incremental Chao: incrementConditionFrame: " << changeChaoFrame << "\n";);
					//loop activates every eighth of chao interval, hitting each stat once per interval
					int statIndex = (changeChaoFrame + 1) / (changeChaoInterval / 8) - 1;//example: (89+1)/(360/8)-1 = 90/45-1 = 1, so FLY stat

					//increment stats on chao
					double timeBalance = statMult * static_cast<double>(changeChaoInterval) / statBase;//to reduce calculations later:
					for (int iterChaoSlots = 0; iterChaoSlots < 24; ++iterChaoSlots) {
						ChaoDataBase* ccdp = &ChaoSlots[iterChaoSlots].data;//ccdp = current chao data pointer
						
						if ((*ccdp).Type >= 2 && (enableAllGardens || (*ccdp).Garden == chaoAreaID)) {//only bother if chao is not egg, in garden scope
							int threeAnimalsIndex = 0;
							int gradeIndex;//the index in the table for getting the value of the max a stat can be
							double statMax = 0.0;

							//get grade index
							if (statIndex < 5) {//visible stats
								gradeIndex = (*ccdp).StatGrades[statIndex] <= 5 ? (*ccdp).StatGrades[statIndex] : 6;
							}
							else {//special case Luck, Intelligence
								gradeIndex = (*ccdp).StatGrades[statIndex] <= 7 ? (*ccdp).StatGrades[statIndex] : 8;
							}

							switch (statIndex) {//get max stat point from index, animal loop indexes
							case 0://swim
								threeAnimalsIndex = 0;
								statMax = gardenStatMax[0][gradeIndex] * (static_cast<double>(gardenStatLevels[0]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax swim: " << statMax << "\n";);
								break;
							case 1://fly
								threeAnimalsIndex = 9;
								statMax = gardenStatMax[0][gradeIndex] * (static_cast<double>(gardenStatLevels[1]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax fly: " << statMax << "\n";);
								break;
							case 2://run
								threeAnimalsIndex = 3;
								statMax = gardenStatMax[1][gradeIndex] * (static_cast<double>(gardenStatLevels[2]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax run: " << statMax << "\n";);
								break;
							case 3://power
								threeAnimalsIndex = 6;
								statMax = gardenStatMax[0][gradeIndex] * (static_cast<double>(gardenStatLevels[3]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax power: " << statMax << "\n";);
								break;
							case 4://stamina
								threeAnimalsIndex = 12;
								statMax = gardenStatMax[0][gradeIndex] * (static_cast<double>(gardenStatLevels[4]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax stamina: " << statMax << "\n";);
								break;
							case 5://luck
								threeAnimalsIndex = 15;
								statMax = gardenStatMaxLI[gradeIndex] * (static_cast<double>(gardenStatLevels[5]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax luck: " << statMax << "\n";);
								break;
							case 6://intelligence
								threeAnimalsIndex = 18;
								statMax = gardenStatMaxLI[gradeIndex] * (static_cast<double>(gardenStatLevels[6]) + 1.0) / 100.0;
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: statMax intelligence: " << statMax << "\n";);
								break;
							}
							int statAnimals = 0;//get animals
							for (int statAnimalsIndex = threeAnimalsIndex; statAnimalsIndex < threeAnimalsIndex + 3; ++statAnimalsIndex) {
								if ((*ccdp).SA2AnimalBehavior >> statAnimalsIndex & 1) statAnimals += 1;
							}
							double preciseStatGain = 0.0;//get the gain for stat
							DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: ccdp.SP[si], statMax: " << (*ccdp).StatPoints[statIndex] << ", " << static_cast<int>(statMax) << "\n";);
							if ((*ccdp).StatPoints[statIndex] != static_cast<int>(statMax)) {
								//Base calculation is 2 hours, so for when garden is not at max rate will take longer
								preciseStatGain = ((1633.0 + (*ccdp).StatGrades[statIndex] * 980.0 / 3.0)) * timeBalance
									* (1.0 + static_cast<double>(statAnimals) / 3.0 + static_cast<double>(gardenHasBeginnerMedal)) / 3.0 + statLeftovers[iterChaoSlots][statIndex];

								int incrementStat = static_cast<int>(preciseStatGain);
								
								if (incrementStat + (*ccdp).StatPoints[statIndex] >= static_cast<int>(statMax)) {
									(*ccdp).StatPoints[statIndex] = static_cast<int>(statMax);
									statLeftovers[iterChaoSlots][statIndex] = 0.0;
									//(*ccdp).StatLevels[statIndex] = 99;//set chao level (see below)
								}
								else {
									(*ccdp).StatPoints[statIndex] += incrementStat;
									statLeftovers[iterChaoSlots][statIndex] = preciseStatGain - static_cast<double>(incrementStat);
									//(*ccdp).StatLevels[statIndex] = 100 * (*ccdp).StatPoints[statIndex] / statMax;
									//set chao level as percent of maxStat (requires save data in unused fields!), make garden levels stored elsewhere
									//would require redundancy as unused fields are reset on chao deletion/creation
								}
								DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: preciseStatGain, leftover: " << preciseStatGain << ", " << statLeftovers[iterChaoSlots][statIndex] << "\n";);
							}
						}
					}
				}
				changeChaoFrame += 1;
				changeChaoFrame %= changeChaoInterval;
			}
			
			DEBUGX(
				if (chaoAreaID == 4 && debugFrame == debugInterval - 1) {
					//logstream << "Incremental Chao: TimerStopped: " << static_cast<int>(TimerStopped) << "\n";
				}
				debugFrame += 1;
				debugFrame %= debugInterval;
			);

			if (!logstream.str().empty()) {
				PrintDebug(logstream.str().c_str());
				logstream.str("");
			}
		}
	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		// Executed before the game processes input
	}

	__declspec(dllexport) void __cdecl OnControl()
	{
		// Executed when the game processes input
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.

	double verifyConfigMult(std::string value) {
		double mult = stod(value);
		if (0 <= mult && mult <= 3) {
			return mult;
		}
		else {
			return 1;
		}
	}

	void doNothing() {
		return;
	}
}

/*
//OLD TESTING CODE
	__int16 curlevID = CurrentLevel;
	if (curlevID == 3 && !dummy) {
		DEBUGX(logstream << "**************green******************\n";);
		dummy = true;
	}
	else if (curlevID == 90 && !dummy) {
		DEBUGX(logstream << "**************chao*******************\n";);
		dummy = true;
	}

	DEBUGX(logstream << "Current Chao Stage is: " << chaoAreaID << "\n";);

	DEBUGX(ccd.DoctorMedal |= 3;);

	DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: 1: " << (1633.0 + (*ccdp).StatGrades[statIndex] * 980.0 / 3.0) << "\n";);
	DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: 2: " << timeBalance << "\n";);
	DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: 3: " << (1.0 + static_cast<double>(statAnimals) / 3.0 + static_cast<double>(gardenHasBeginnerMedal)) << "\n";);
	DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: 4: " << 3.0 << "\n";);
	DEBUGX(if (iterChaoSlots == 0) logstream << "Incremental Chao: 5: " << statLeftovers[iterChaoSlots][statIndex] << "\n";);
*/