/*
 * polyReg.h
 *
 *  Created on: Aug 12, 2023
 *      Author: raphael
 */

#ifndef POLYREG_H_
#define POLYREG_H_

#include <vector>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>


using namespace std;


namespace Regression{

void printMatrix(double ** Matrix, int row, int col){
		for(int i = 0 ; i< row; i++){
			for(int j = 0 ; j< col; j++){
				std::cout<<Matrix[i][j]<< "\t";
			}
			std::cout<<"\n";
		}
	}

double** inverseFinder(double** matrix, int size){
	double ** ret = new double*[size];
	for(int i = 0 ; i< size; i++){
		ret[i] = new double[size];
	}

	for(int i = 0 ; i< size; i++){
		for(int j = 0 ; j< size; j++){
			if(i==j){
				ret[i][j] = 1;
			}
			else{
				ret[i][j] = 0;
			}
		}
	}

	for(int i = 0 ; i<size ; i++){
		if(matrix[i][i]==0){
			for(int j = i+1; j<size; j++){
				if(matrix[j][j]!=0){
					for(int k = 0; k< size; k++){
						double temp = matrix[i][k];
						matrix[i][k] = matrix[j][k];
						matrix[j][k] = temp;
						temp = ret[i][k];
						ret[i][k] = ret[j][k];
						ret[j][k] = temp;
					}
				}
			}
			if(matrix[i][i] == 0){
				std::cout<<"ERROR";
			}
		}
		double scalar = 1/(matrix[i][i]);
		for(int j = 0; j<size ; j++){
			matrix[i][j] *= scalar;
			ret[i][j] *= scalar;
		}

		for(int j = 0; j<size; j++){
			if(j!=i){
				scalar = -1*matrix[j][i];
				for(int k = 0; k<size; k++){
					matrix[j][k] += matrix[i][k]*scalar;
					ret[j][k] += ret[i][k]*scalar;
				}
			}
		}
	}
	return ret;
}
	double multiplyMatricesHelper(double** MatrixA, double** MatrixB, int row, int col, int length){
		cout<<row<<" "<<col<<" "<<length<<"\n";
		double ret = 0;
			for(int i = 0; i<length; i++){
				ret+= MatrixA[row][i]* MatrixB[i][col];
			}
			return ret;
		}

	double ** multiplyMatrices(double** MatrixA, double** MatrixB, int rowA, int colA, int rowB, int colB){
		double ** ret = new double*[rowA];
		cout<<rowA<<" "<<colA<<" "<<rowB<<" "<<colB<<"\n";
		printMatrix(MatrixA, rowA, colA);
		printMatrix(MatrixB, rowB , colB);
		cout<<"issueM0 \n";
		for(int i = 0 ; i< rowA; i++){
			ret[i] = new double[colB];
		}
		cout<<"issueM1 \n";
		for(int i = 0 ; i< rowA; i++){
			for(int j = 0; j< colB; j++){
				ret[i][j] = multiplyMatricesHelper(MatrixA, MatrixB, i, j, colA);
			}
			cout<<"issueM2 \n";
		}
		return ret;
	}
	double multiplyVectorHelper(double**Matrix, double* Vect, int row, int length){
		double ret = 0;
		for(int i = 0 ; i < length ; i++){
			ret+= Matrix[row][i]*Vect[i];
		}

		return ret;
	}
	double* multiplyVector(double** Matrix, double* Vect, int rowA, int colA, int rowB){
		double* ret = new double[rowA];
		for(int i = 0 ; i< rowA; i++){
			ret[i] = multiplyVectorHelper(Matrix, Vect, i, colA);
		}
		return ret;
	}

	double * VectToPoint(vector<double> change, int size){
		double*ret = new double[size];
		for(int i = 0 ; i< size; i++){
			ret[i] = change[i];
		}
		return ret;
	}

	double** makeMatrixA(vector<double> XValues, int degree, int size){
		double** ret = new double *[size];
		for(int i = 0 ; i< size; i++){
			ret[i] = new double[degree+1];
		}
		for(int i = 0 ; i< size; i++){
			for(int j = 0 ; j< degree+1; j++){
				ret[i][j] = std::pow(XValues[i], j);
			}
		}
		return ret;
	}

	double** transposeMatrix(double** MatrixA, int row, int col){
			double**ret = new double *[col];
			for(int i = 0 ; i< col; i++){
				ret[i] = new double[row];
			}
			int dexRow = 0;
			int dexCol = 0;
			while(dexRow< row && dexCol<col){
				ret[dexCol][dexRow] = MatrixA[dexRow][dexCol];
				dexRow++;
				if(dexRow==row){
					dexRow =0;
					dexCol++;
				}
			}
			return ret;
		}
}




#endif /* POLYREG_H_ */
