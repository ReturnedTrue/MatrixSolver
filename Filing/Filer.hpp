#pragma once

#include <string>

#include "../Lib/Utility.hpp"

using std::string;
using std::stringstream;

#define FILER_NO_FILE_FOUND "No such file"
#define FILER_READ_FAILED "Failed to read file"

namespace Filer {
	std::pair<bool, string> loadFileContents(string fileName) {
		auto file = std::fopen(fileName.c_str(), "r");
		if (!file) {
			return {false, FILER_NO_FILE_FOUND};
		}
		
		string fullString;
		int currentCharacter;

		while ((currentCharacter = std::fgetc(file)) != EOF) {
			fullString = fullString + char(currentCharacter);
		}

		pair<bool, string> response;
		
		if (std::ferror(file)) {
			response = {false, FILER_READ_FAILED};
		} else {
			response = {true, fullString};
		}

		std::fclose(file);
		return response;
	}

	std::pair<bool, string> loadFileContentsWithRetries(string fileName, string retryMessage) {
		do {
			auto result = loadFileContents(fileName);
			
			if (!result.first) {
				if (result.second == FILER_NO_FILE_FOUND) {
					return result;
				}

				continue;
			}

			return result;
		} while (Utility::promptYesNo(retryMessage));

		return {false, ""};
	}

	bool writeFileContents(string fileName, string newContents) {
		auto file = std::fopen(fileName.c_str(), "w+");
		if (!file) return false;

		std::fwrite(newContents.data(), sizeof(char), newContents.length(), file);
		std::fclose(file);

		return true;
	}

	bool writeFileContentsWithRetries(string fileName, string newContents, string retryMessage) {
		do {
			if (writeFileContents(fileName, newContents)) {
				return true;
			}
		} while (Utility::promptYesNo(retryMessage));

		return false;
	}

	void removeFile(string fileName) {
		std::remove(fileName.c_str());
	}
} 