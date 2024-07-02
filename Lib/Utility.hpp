#pragma once

#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <functional>
#include <stdexcept>
#include <locale>
#include <sstream>
#include "../Matrices/MatrixTypes.hpp"

using std::string;
using std::stringstream;
using std::pair;
using std::map;
using std::function;

namespace Utility {
	void println() {
		std::cout << std::endl;
	}

	void println(string message) {
		std::cout << message << std::endl;
	}

	double promptNumber(string message) {
		std::cout << message;

		while (true) {
			string input;
			std::cin >> input;

			try {
				return std::stod(input);
			} catch(std::exception e) {
				Utility::println("Please input a number value!");
			}
		}
	}

	string promptString(string message) {
		std::cout << message;

		string input;
		std::cin >> input;

		return input;
	}

	string stringToLowercase(string str) {
		string lowerForm;
		
		for (auto character : str) {
			lowerForm += std::tolower(character);
		}

		return lowerForm;
	}

	vector<string> splitStringByCharacter(string str, char character) {
		vector<string> values;

		stringstream stream(str);
		string nextValue;

		while (getline(stream, nextValue, character)) {
			values.push_back(nextValue);
		}

		return values;
	}

	bool promptYesNo(string message) {
		string response = promptString(message + "(y/n): ");
		string lowerForm = stringToLowercase(response);

		return lowerForm == "y" || lowerForm == "yes";
	}

	string toStringWithSigFig(double x, double n) {
	    if (x == 0.0)
	        return "0.0";
	
	    double factor = pow(10.0, n - ceil(log10(fabs(x))));
	    double finalResult = round(x * factor) / factor;

		string str = std::to_string(finalResult);
		str.erase(str.find_last_not_of('0') + 1, string::npos);
		str.erase(str.find_last_not_of('.') + 1, string::npos);

		return str;
	}

	void tryFuncAndReportError(function<void()> func) {
		try {
			func();
		} catch(string e) {
			println(e);
		} catch (const char* e) {
			println("Dev Bug: outputted a const char*, must change to string()");
			println(e);
		}
	}

	template<typename V>
	pair<bool, V*> tryGetFromVector(vector<V>* currentVec, int index) {
		try {
			return {true, &currentVec->at(index)};
		} catch(std::out_of_range e) {
			return {false, nullptr};
		}
	}

	template<typename K, typename V>
	pair<bool, V*> tryGetFromMap(map<K, V>* currentMap, K key) {
		try {
			return {true, &currentMap->at(key)};
		} catch (std::out_of_range e) {
			return {false, nullptr};
		}
	}

	template<typename T = double>
	vector<vector<T>> getElementsForSize(int rows, int columns) {
		vector<vector<T>> elements;
		elements.resize(rows);

		for (int i = 0; i < rows; i++) {
			elements[i].resize(columns);
		}

		return elements;
	}

	template<typename T>
	string concatMapKeys(map<string, T>* currentMap) {
		string result = "";
		bool isFirst = true;
	
		for (const auto& data : (*currentMap)) {
			if (!isFirst) result += ", ";
			else isFirst = false;
			
			result += data.first;
		}

		return result;
	}
}
