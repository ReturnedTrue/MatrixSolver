#pragma once

#include "Filer.hpp"
#include "../Lib/Utility.hpp"

#define SETTINGS_FILE "./solver_settings.txt"

typedef struct {
	bool promptSave;
	bool promptClear;
} Settings;

namespace SettingsManager {
	string serializeBoolean(bool data) {
		if (data == true) return "TRUE";

		return "FALSE";
	}

	bool deserializeBoolean(string data) {
		if (data == "TRUE") return true;
		
		return false;
	}

	Settings loadSettings() {
		Settings settings;

		settings.promptSave = true;
		settings.promptClear = true;
		
		auto loadResult = Filer::loadFileContentsWithRetries(
			SETTINGS_FILE, 
			"Failed to load settings, try again? "
		);

		if (!loadResult.first) return settings;

		auto lines = Utility::splitStringByCharacter(loadResult.second, '\n');
		vector<string> stringValues;

		for (auto line : lines) {
			auto lineMembers = Utility::splitStringByCharacter(line, '=');
			stringValues.push_back(lineMembers[1]);
		}

		settings.promptSave = deserializeBoolean(stringValues[0]);
		settings.promptClear = deserializeBoolean(stringValues[1]);

		Utility::println("Loaded settings");
		return settings;
	}

	void saveSettings(Settings* settings) {
		vector<pair<string, string>> lines = {
			{"prompt-save", serializeBoolean(settings->promptSave)},
			{"prompt-clear", serializeBoolean(settings->promptClear)}
		};

		string serializedSettings;

		for (auto line : lines) {
			serializedSettings += line.first + "=" + line.second + "\n";
		}
		
		auto success = Filer::writeFileContentsWithRetries(
			SETTINGS_FILE,
			serializedSettings,
			"Failed to save settings, try again? "
		);

		if (success) {
			Utility::println("Saved settings");
		}
	}
}