#pragma once

#include <string>
#include <vector>
#include "BaseMatrix.hpp"
#include "MatrixTypes.hpp"
#include "Matrix2D.hpp"
#include "../Lib/Utility.hpp"

using std::string;
using std::vector;

class Matrix3D : public BaseMatrix {
	public:
		Matrix3D() : BaseMatrix(3, 3) {}
		Matrix3D(SingleLayerElements itemList) : BaseMatrix(3, 3, itemList) {}

		double getDeterminant() {
			auto firstMinor = this->getMinorByEliminating(0, 0);
			auto secondMinor = this->getMinorByEliminating(0, 1);
			auto thirdMinor = this->getMinorByEliminating(0, 2);

			auto currentElements = this->elements;
			
			return 
				(firstMinor.getDeterminant() * currentElements[0][0])
				- (secondMinor.getDeterminant() * currentElements[0][1])
				+ (thirdMinor.getDeterminant() * currentElements[0][2]);
		}

		MatrixElements getInverseElements() {
			// Matrix of minors, matrix of cofactors, then tranpose, multiply by 1 / det

			double determinant = this->getDeterminant();
			if (determinant == 0) throw string("Matrix is singular");
			
			double reciprocalDeterminant = 1 / determinant;
			auto minorMatrix = this->getMatrixOfMinors();
			auto cofactorMatrix = minorMatrix.getMatrixOfCofactors();
			auto tCofactorMatrixElements = cofactorMatrix.getTransposedElements();

			return tCofactorMatrixElements;
		}

	private:
		Matrix2D getMinorByEliminating(int row, int column) {
			SingleLayerElements minorElements;

			for (int i = 0; i < 3; i++) {
				if (i == row) continue;
				
				for (int j = 0; j < 3; j++) {
					if (j == column) continue;
					minorElements.push_back(this->elements[i][j]);
				}
			}

			return Matrix2D(minorElements);
		}

		Matrix3D getMatrixOfMinors() {
			auto newMatrix = Matrix3D();

			for (int i = 0; i < 3; i++) {		
				for (int j = 0; j < 3; j++) {
					auto minor = this->getMinorByEliminating(i, j);	
					newMatrix.setElement(i, j, minor.getDeterminant());
				}
			}

			return newMatrix;
		}

		Matrix3D getMatrixOfCofactors() {
			auto newMatrix = Matrix3D();
			bool shouldFlip = false;
			
			for (int i = 0; i < 3; i++) {		
				for (int j = 0; j < 3; j++) {
					double cofactor = shouldFlip ? -1 : 1;
					
					newMatrix.setElement(i, j, this->elements[i][j] * cofactor);
					shouldFlip = !shouldFlip;
				}
			}

			return newMatrix;
		}
};