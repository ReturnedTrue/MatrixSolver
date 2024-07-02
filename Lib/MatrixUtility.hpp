#pragma once

#include "Utility.hpp"
#include "../Matrices/BaseMatrix.hpp"
#include "../Matrices/Matrix2D.hpp"
#include "../Matrices/Matrix3D.hpp"
#include "../Matrices/MatrixTypes.hpp"

using Utility::println;
using std::to_string;

namespace MatrixUtility {
	BaseMatrix* getMatrixForSize(int rows, int columns, bool doPrint) {
		BaseMatrix* matrix = nullptr;
		
		if (rows == 2 && columns == 2) {
			if (doPrint) println("[Square matrix selected]");
			matrix = new Matrix2D();
			
		} else if (rows == 3 && columns == 3) {
			if (doPrint) println("[Cube matrix selected]");
			matrix = new Matrix3D();
	
		} else {
			if (doPrint) {
				println("[" + to_string(rows) + "x" + to_string(columns) + " matrix selected]");
			}
			
			matrix = new BaseMatrix(rows, columns);
		}
	
		return matrix;
	}

	BaseMatrix* getMatrixForElements(MatrixElements elements) {
		int rows = elements.size();
		int columns = elements[0].size();
		
		auto matrix = getMatrixForSize(rows, columns, false);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				matrix->setElement(i, j, elements[i][j]);
			}
		}

		return matrix;
	}

	bool areTheSameSize(BaseMatrix* a, BaseMatrix* b) {
		return a->rows == b->rows && a->columns == b->columns;
	}

	BaseMatrix* addMatrices(BaseMatrix* a, BaseMatrix* b) {
		if (!areTheSameSize(a, b)) {
			throw string("Not additively conformable");
		}

		int rows = a->rows;
		int columns = a->columns;
		auto resultMatrix = getMatrixForSize(rows, columns, false);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				auto addResult = a->getElement(i, j) + b->getElement(i, j);
				resultMatrix->setElement(i, j, addResult);
			}
		}

		return resultMatrix;
	}

	BaseMatrix* subtractMatrices(BaseMatrix* a, BaseMatrix* b) {
		if (!areTheSameSize(a, b)) {
			throw string("Not subtractively conformable");
		}

		int rows = a->rows;
		int columns = a->columns;
		auto resultMatrix = getMatrixForSize(rows, columns, false);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				auto addResult = a->getElement(i, j) - b->getElement(i, j);
				resultMatrix->setElement(i, j, addResult);
			}
		}

		return resultMatrix;
	}

	BaseMatrix* multiplyMatrices(BaseMatrix* a, BaseMatrix* b) {
		if (a->columns != b->rows) {
			throw string("Not multiplicatively conformable");
		}

		auto resultMatrix = getMatrixForSize(a->rows, b->columns, false);

		for (int i = 0; i < a->rows; i++) {
			for (int j = 0; j < b->columns; j++) {
				double elementResult = 0;

				for (int k = 0; k < b->rows; k++) {
					elementResult += a->getElement(i, k) * b->getElement(k, j);
				}
				
				resultMatrix->setElement(i, j, elementResult);
			}
		}

		return resultMatrix;	
	}
}