#pragma once

#include <vector>
#include <string>
#include "MatrixTypes.hpp"
#include "../Lib/Utility.hpp"

using std::vector;
using std::string;
using std::to_string;

class BaseMatrix {
	protected:
		MatrixElements elements;

	public:
		int rows;
		int columns;

		BaseMatrix(int rows, int columns, SingleLayerElements singleLayerList) {
			auto elements = Utility::getElementsForSize(rows, columns);

			int count = 0;

			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					elements[i][j] = singleLayerList[count];
					count++;
				}
			}

			this->rows = rows;
			this->columns = columns;
			this->elements = elements;
		}

		BaseMatrix(int rows, int columns) {
			this->rows = rows;
			this->columns = columns;

			this->elements = Utility::getElementsForSize(rows, columns);
		}

		string visualize() {
			string str = "";
			vector<int> sizeForEachColumn;
			sizeForEachColumn.resize(this->columns);
			
			auto repResults = Utility::getElementsForSize<string>(this->rows, this->columns);

			for (int i = 0; i < this->rows; i++) {
				for (int j = 0; j < this->columns; j++) {
					auto item = this->elements[i][j];
					
					auto rep = Utility::toStringWithSigFig(item, 3);
					repResults[i][j] = rep;

					auto size = rep.size();

					if (size > sizeForEachColumn[j]) {
						sizeForEachColumn[j] = size;
					}
				}
			}	
			
			for (int i = 0; i < this->rows; i++) {
				for (int j = 0; j < this->columns; j++) {
					auto rep = repResults[i][j];
					auto columnSize = sizeForEachColumn[j];
					auto neededPadding = abs(int(rep.size() - columnSize)) + 1;
					//auto stringPadding = neededPadding > 0 ? string(neededPadding, ' ') : " "; 
					
					str += "| " + rep + string(neededPadding, ' ');
				}

				str += "|\n";
			}

			return str;
		}

		double getElement(int row, int column) {
			return this->elements[row][column];
		}

		void setElement(int row, int column, double item) {
			this->elements[row][column] = item;
		}

		double virtual getDeterminant() {
			throw string("Cannot solve the determinant for this type of matrix");
		}

		MatrixElements virtual getInverseElements() {
			throw string("Cannot solve the inverse for this type of matrix");
		}

		MatrixElements virtual getTransposedElements() {
			auto newElements = Utility::getElementsForSize(this->columns, this->rows);

			for (int i = 0; i < this->rows; i++) {
				for (int j = 0; j < this->columns; j++) {
					newElements[j][i] = this->elements[i][j];
				}
			}

			return newElements;
		}
};

typedef BaseMatrix* AnyMatrix;