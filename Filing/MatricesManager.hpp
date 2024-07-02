#pragma once

#include "Filer.hpp"
#include "../Lib/Utility.hpp"
#include "../Lib/MatrixUtility.hpp"
#include "../Matrices/BaseMatrix.hpp"

#define MATRIX_FILE "./saved_matrices.txt"

typedef map<string, BaseMatrix*> Matrices;

namespace MatricesManager {
	string serializeMatrix(string identifier, BaseMatrix* matrix) {
		string result = identifier + "," + to_string(matrix->rows) + "," + to_string(matrix->columns);
		
		for (int i = 0; i < matrix->rows; i++) {
			for (int j = 0; j < matrix->columns; j++) {
				result += "," + to_string(matrix->getElement(i, j));
			}
		}
		
		return result;
	}
	
	std::pair<string, BaseMatrix*> deserializeMatrix(string serialized) { 
		auto values = Utility::splitStringByCharacter(serialized, ',');

		string identifier = values[0];
		int rows = std::stoi(values[1]);
		int columns = std::stoi(values[2]);
		int nextIndex = 3;

		auto matrix = MatrixUtility::getMatrixForSize(rows, columns, false);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				double converted = std::stod(values[nextIndex]);
				matrix->setElement(i, j, converted);

				nextIndex++;
			}
		}

		return {identifier, matrix};
	}

	Matrices loadMatrices() {
		Matrices matrices;

		auto loadResponse = Filer::loadFileContentsWithRetries(
			MATRIX_FILE,
			"Failed to load matrices, try again? "
		);
		
		if (!loadResponse.first) {
			return {};
		}
		
		string contents = loadResponse.second;
		auto differentMatrices = Utility::splitStringByCharacter(contents, '\n');
	
		if (differentMatrices.empty()) {
			return {};
		}
	
		for (auto serializedMatrix : differentMatrices) {
			auto deserializeResponse = deserializeMatrix(serializedMatrix);
			(matrices)[deserializeResponse.first] = deserializeResponse.second;
		}
	
		Utility::println("Loaded stored matrices: " + Utility::concatMapKeys(&matrices));
		return matrices;
	}
	
	void saveMatrices(Matrices* matrices) {
		string contents = "";
	
		for (auto matrixData : *matrices) {
			contents += serializeMatrix(matrixData.first, matrixData.second) + "\n";
		}
	
		bool success = Filer::writeFileContentsWithRetries(
			MATRIX_FILE, 
			contents, 
			"Failed to save matrices, try again? "
		);
	
		if (success) {
			Utility::println("Stored matrices successfully");
		}
	}
}