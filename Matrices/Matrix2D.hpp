#pragma once

#include <string>
#include <vector>
#include "BaseMatrix.hpp"
#include "MatrixTypes.hpp"
#include "../Lib/Utility.hpp"

using std::string;
using std::vector;

class Matrix2D : public BaseMatrix {
	public:
		Matrix2D() : BaseMatrix(2, 2) {}
		Matrix2D(SingleLayerElements itemList) : BaseMatrix(2, 2, itemList) {}

		double getDeterminant() {
			auto currentElements = this->elements;
			double firstDiagonal = currentElements[0][0] * currentElements[1][1];
			double secondDiagonal = currentElements[0][1] * currentElements[1][0];
			
			return firstDiagonal - secondDiagonal;
		}

		MatrixElements getInverseElements() {
			double determinant = this->getDeterminant();
			if (determinant == 0) throw string("Matrix is singular");

			double reciprocalDeterminant = 1 / determinant;
			auto currentElements = this->elements;

			MatrixElements newElements = {
				{currentElements[1][1] * reciprocalDeterminant, currentElements[0][1] * -1 * reciprocalDeterminant},
				{currentElements[1][0] * -1 * reciprocalDeterminant, currentElements[0][0] * reciprocalDeterminant}
			};

			return newElements;
		}
};