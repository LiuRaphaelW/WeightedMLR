/*
 * MLR.h
 *
 *  Created on: Aug 11, 2023
 *      Author: raphael
 */

#include "polyReg.h"
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



#ifndef MLR_H_
#define MLR_H_

class MLR {
};

class weightingHelper{
	public:
		weightingHelper(){totalWeight = 0;};
		vector<double> predictions;
		vector<double> weights;
		double totalWeight;
		void calculateTotalWeight(){
			totalWeight = 0;
			for(int i = 0 ; i< weights.size(); i++){
				totalWeight+= weights[i];
			}
		}
		double projection(){
			double ret = 0;
			calculateTotalWeight();
			for(int i = 0; i< predictions.size(); i++){
				ret += predictions[i]*(weights[i]/totalWeight);
			}
			return ret;
		}
};

class columnInfo{
	public:
		string name = "";
		vector<double>data;
		int numDataPoints;
		columnInfo(){numDataPoints = 0;};

};

class regressionInfo{
	public:
		regressionInfo(){degree = 1; rmse = 0; mse = 0; mae = 0;};
		columnInfo independentVar;
		columnInfo dependentVar;
		double rmse;
		double mse;
		double mae;
		vector<double> solutionVector;
		int degree;
		void runRegression(){
			double* VectB = Regression::VectToPoint(dependentVar.data, dependentVar.numDataPoints);
			cout<<"issue0 \n"<<degree<<"\n";
			double ** MatrixA = Regression::makeMatrixA(independentVar.data, degree, independentVar.numDataPoints);
			cout<<"issue1 \n";
			double ** MatrixTranspose = Regression::transposeMatrix(MatrixA, independentVar.numDataPoints,2);
			cout<<"issue2 \n";
			double ** MatrixInverse = Regression::inverseFinder(Regression::multiplyMatrices(MatrixTranspose, MatrixA, degree+1, independentVar.numDataPoints, independentVar.numDataPoints, degree+1), degree+1);
			cout<<"issue3 \n";
			double * solutionVectorTemp = Regression::multiplyVector(Regression::multiplyMatrices(MatrixInverse, MatrixTranspose, degree+1,degree+1, degree+1, independentVar.numDataPoints), VectB, degree+1, independentVar.numDataPoints, independentVar.numDataPoints);
			cout<<"issue4 \n";
			vector<double> doubleVector(solutionVectorTemp, solutionVectorTemp+degree+1);
			cout<<"issue5 \n";
			solutionVector = doubleVector;
			cout<<"issue6 \n";
			delete(solutionVectorTemp);
			delete(VectB);
			delete(MatrixA);
			delete(MatrixTranspose);
			delete(MatrixInverse);

		}
		void calculateMSE(){
			for(int i = 0 ; i < dependentVar.numDataPoints; i++){
				mse += std::pow(dependentVar.data[i] - predict(independentVar.data[i]),2);
			}
			mse /= dependentVar.numDataPoints;
		}
		void calculateRMSE(){
			for(int i = 0 ; i < dependentVar.numDataPoints; i++){
								rmse += std::pow(dependentVar.data[i] - predict(independentVar.data[i]),2);
			}
			rmse = std::sqrt(rmse)/dependentVar.numDataPoints;
		}
		void calculateMAE(){
			for(int i = 0 ; i < dependentVar.numDataPoints; i++){
				mae += std::abs(dependentVar.data[i] - predict(independentVar.data[i]));
			}
			mae /= dependentVar.numDataPoints;
		}
		double predict(double val){
			double ret;
			for(int i = 0 ; i< degree+1; i++){
				ret += solutionVector[i]*std::pow(val,i);
			}
			return ret;
		}
};
#endif /* MLR_H_ */
