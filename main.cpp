#include <iostream>
#include <functional>
#include <utility>
#include "Lib/Utility.hpp"
#include "Lib/MatrixUtility.hpp"
#include "Matrices/BaseMatrix.hpp"
#include "Matrices/MatrixTypes.hpp"
#include "Filing/Filer.hpp"
#include "Filing/SettingsManager.hpp"
#include "Filing/MatricesManager.hpp"

#define MATRIX_FILE "./saved_matrices.txt"

using std::map;
using std::pair;
using std::function;
using std::to_string;

using Utility::promptNumber;
using Utility::promptString;
using Utility::println;
using Utility::tryFuncAndReportError;

typedef map<string, function<BaseMatrix*(BaseMatrix*, BaseMatrix*)>> Operations;

typedef pair<string, function<void(Matrices*, Settings*)>> MenuData;
typedef vector<MenuData> Menus;

BaseMatrix* promptMatrix() {
	double rows = promptNumber("Rows: ");
	double columns = promptNumber("Columns: ");

	auto matrix = MatrixUtility::getMatrixForSize(rows, columns, true);
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			double element = promptNumber(
				"Element [" + to_string(i) + "][" + to_string(j) + "]: "
			);
			
			matrix->setElement(i, j, element);
		}
	}

	return matrix;
}

string promptIdentifier(string message) {
	while (true) {
		string identifier = promptString(message);

		if (identifier.find("\n") != string::npos) {
			println("Identifiers cannot include new lines");
			
		} else if (identifier.find(",") != string::npos) {
			println("Identifiers cannot include commas");
			
		} else {
			return identifier;
		}
	}
}

pair<string, BaseMatrix*> promptMatrixIdAndRetrieve(Matrices* matrices, string message) {
	while (true) {
		string identifier = promptIdentifier(message);
		auto tryResponse = Utility::tryGetFromMap(matrices, identifier);

		if (tryResponse.first) {
			return {identifier, *tryResponse.second};
		}

		println("Matrix '" + identifier + "' does not exist");
	}
}

void promptMatrixDefinition(Matrices* matrices, string message, BaseMatrix* suppliedMatrix) {
	string identifier = promptIdentifier(message);

	auto preexisting = Utility::tryGetFromMap(matrices, identifier);
	if (preexisting.first) {
		free(*preexisting.second);
	}
	
	(*matrices)[identifier] = suppliedMatrix != nullptr ? suppliedMatrix : promptMatrix();
}

void defineMatrixMenu(Matrices* matrices, Settings* settings) {
	promptMatrixDefinition(matrices, "New matrix identifier: ", nullptr);
}

void inspectMatrixMenu(Matrices* matrices, Settings* settings) {
	if (matrices->empty()) {
		println("No matrices are defined");
		return;
	}

	println("Defined matrices: " + Utility::concatMapKeys(matrices));

	auto matrixResponse = promptMatrixIdAndRetrieve(matrices, "Matrix identifier: ");
	auto identifier = matrixResponse.first;
	auto matrix = matrixResponse.second;

	string sizeString = to_string(matrix->rows) + "x" + to_string(matrix->columns);
	println("Inspecting matrix " + identifier + " which is " + sizeString);

	tryFuncAndReportError([matrix]() {
		string determinant = Utility::toStringWithSigFig(matrix->getDeterminant(), 3);
		println("Determinant: " + determinant);
	});

	println("Visualized:\n" + matrix->visualize());

	auto transposed = MatrixUtility::getMatrixForElements(matrix->getTransposedElements());
	println("Tranposed:\n" + transposed->visualize());

	bool saveTransposed = Utility::promptYesNo("Save tranposed matrix? ");
	if (saveTransposed) {
		promptMatrixDefinition(matrices, "Tranposed matrix identifier: ", transposed);
	}

	tryFuncAndReportError([matrices, matrix]() {
		auto inverse = MatrixUtility::getMatrixForElements(matrix->getInverseElements());
		println("Inverted:\n" + inverse->visualize());

		bool saveInverse = Utility::promptYesNo("Save inverted matrix? ");
		if (saveInverse) {
			promptMatrixDefinition(matrices, "Inverted matrix identifier: ", inverse);
		}
	});
}

void calculateMatrixMenu(Matrices* matrices, Settings* settings) {
	if (matrices->empty()) {
		println("No matrices are defined");
		return;
	}

	Operations static operations = {
		{"+", MatrixUtility::addMatrices},
		{"-", MatrixUtility::subtractMatrices},
		{"*", MatrixUtility::multiplyMatrices}
	};

	println("Defined matrices: " + Utility::concatMapKeys(matrices));

	auto firstMatrix = promptMatrixIdAndRetrieve(matrices, "Matrix 1: ").second;
	auto secondMatrix = promptMatrixIdAndRetrieve(matrices, "Matrix 2: ").second;

	println("Possible operations: " + Utility::concatMapKeys(&operations));

	string operation = promptString("Operation: ");
	auto operationRetrieveResult = Utility::tryGetFromMap(&operations, operation);
	if (!operationRetrieveResult.first) return;

	auto operationFunction = *operationRetrieveResult.second;

	tryFuncAndReportError([matrices, operationFunction, firstMatrix, secondMatrix]() {
		auto calculatedMatrix = operationFunction(firstMatrix, secondMatrix);
		println("Result:\n" + calculatedMatrix->visualize());	

		bool saidYes = Utility::promptYesNo("Would you like to save this matrix? ");
		if (saidYes) {
			promptMatrixDefinition(matrices, "Saved matrix identifier: ", calculatedMatrix);
		}
	});
}

void saveMatricesMenu(Matrices* matrices, Settings* settings) {
	if (matrices->empty()) {
		println("No matrices are defined");
		return;
	}

	MatricesManager::saveMatrices(matrices);
}

void clearMatricesMenu(Matrices* matrices, Settings* settings) {
	if (settings->promptClear) {
		if (!Utility::promptYesNo("Are you sure you wish to clear all matrices? ")) return;
	}
	
	*matrices = {};
	Filer::removeFile(MATRIX_FILE);

	println("Matrices cleared");
}

void editSettingsMenu(Matrices* matrices, Settings* settings) {
	settings->promptSave = Utility::promptYesNo("Prompt to save matrices on quit? ");
	settings->promptClear = Utility::promptYesNo("Prompt to ensure you want to clear matrices? ");
}

void exitMenu(Matrices* matrices, Settings* settings) {
	if (!matrices->empty() && settings->promptSave && Utility::promptYesNo("Save matrices? ")) {
		MatricesManager::saveMatrices(matrices);
	}

	SettingsManager::saveSettings(settings);
	
	std::exit(0); 
}

int main() {
	Menus menus = {
		{"Define Matrix", defineMatrixMenu},
		{"Inspect Matrix", inspectMatrixMenu},
		{"Calculate Matrix", calculateMatrixMenu},
		{"Save Matrices", saveMatricesMenu},
		{"Clear Matrices", clearMatricesMenu},
		{"Edit Settings", editSettingsMenu},
		{"Quit", exitMenu}
	};

	Matrices matrices = MatricesManager::loadMatrices();
	Settings settings = SettingsManager::loadSettings();
	
	while (true) {
		println("===== Matrix Solver =====");

		int count = 0;
		for (auto menuData : menus) {
			auto menuChoice = to_string(count + 1) + ". ";
			println(menuChoice + menuData.first);

			count++;
		}

		int selectedChoice = promptNumber("Choice: ") - 1;
		auto menuDataResult = Utility::tryGetFromVector(&menus, selectedChoice);

		if (!menuDataResult.first) {
			println("Please select a valid menu");
			continue;
		}

		auto menuData = *menuDataResult.second;

		println("=> " + menuData.first + " <=");
		menuData.second(&matrices, &settings);	
	}
}
