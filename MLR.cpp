/*
 * MLR.cpp
 *
 *  Created on: Aug 11, 2023
 *      Author: raphael
 */

#include "MLR.h"


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
#include <algorithm>


using namespace std;

int StringEqual(string A, string B){
//	cout<<"b1 \n";
	if(A.size() != B.size()){
		return 0;
	}
//	cout<<"b2 \n";
	for(int i = 0; i < A.size(); i++){
		if(A[i]!= B[i]){
//			cout<<"b3 \n";
			return 0;
		}
	}
	return 1;
}

bool isString(string test){
	if(test.size() !=0 && ((int)test[0]>57 ||(int)test[0]<48)&&((int)test[test.size()-1]>57 || (int)test[test.size()-1]<48)){
		return true;
	}
	return false;
}
void readFile(vector<columnInfo>& TrainColumnData, const string& fileString){
	std::ifstream file(fileString);
	if(!file.is_open()){
		cout<<"failed to open file \n";
		return;
	}
	string readLine;
	string check;
	getline(file, readLine);
	string tempVal;
	stringstream s(readLine);
	std::getline(s,tempVal,',');
	while(isString(tempVal)&& check != tempVal){
		columnInfo temp;
		temp.name = tempVal;
		TrainColumnData.push_back(temp);
		check = tempVal;
		getline(s,tempVal,',');
		cout<<temp.name;
	}
	TrainColumnData[TrainColumnData.size()-1].name = TrainColumnData[TrainColumnData.size()-1].name.substr(0,TrainColumnData[TrainColumnData.size()-1].name.size()-1);
	cout<<"\n";
	int indexData= 0;
	while(getline(file,readLine)){
		stringstream s(readLine);
		for(int i = 0 ; i< TrainColumnData.size(); i++){
			getline(s,tempVal,',');
			TrainColumnData[i].data.push_back(stod(tempVal));
			cout<<tempVal<<"\t";
		}
		cout<<"\n";
		indexData++;
	}
	for(int i = 0; i< TrainColumnData.size(); i++){
		TrainColumnData[i].numDataPoints = indexData;
		cout<<TrainColumnData[i].data.size()<<"\n";
	}
	cout<<TrainColumnData.size()<<"\n"<<indexData+1<<"\n";
}


int main(int argc, char * argv[]){
	std::string fileString = "data.csv";
	vector<columnInfo> TrainColumnData;
	vector<regressionInfo> regressionData;
	vector<vector<double>> solutionVectors;
	std::string lineRead;
	weightingHelper RMSEWH;
	weightingHelper MSEWH;
	weightingHelper MAEWH;
	int dependentVariable;
	readFile(TrainColumnData, fileString);
//	cout<<TrainColumnData[0].name.size();cout<<"\n"<<TrainColumnData[1].name.size()<<"\n"<<TrainColumnData[].name.size()<<"\n";
	cout<<"what is your dependent variable? \n";
	int isEqual = 0;
//	cout<<isEqual<<"\n";
	while(isEqual==0){
		getline(cin, lineRead);
//		cout<<lineRead<<"\n";
		for(int i = 0 ; i< TrainColumnData.size(); i++){
			isEqual =   StringEqual(TrainColumnData[i].name, lineRead);
//			cout<<TrainColumnData[i].name<<"\n"<<i<<"\n"<<isEqual<<"\n";
			if(isEqual){
//				cout<<"got here"<<"\n";
				dependentVariable = i;
//				cout<<"got here"<<"\n";
				break;
			}
		}
//		cout<<isEqual<<"\n";
		if(isEqual==0){
			cout<<"please enter a valid category name \n";
		}
	}
//	cout<<"made it"<<"\n";
	for(int i = 0 ; i< TrainColumnData.size(); i++){
		if(i != dependentVariable){
			vector<regressionInfo> tempRegressions;
			for(int j = 0; j <= (int)(sqrt(TrainColumnData[0].numDataPoints)/75); j++){
				cout<<"issueL0 \n";
				regressionInfo temp;
				cout<<"issueL1 \n";
				temp.dependentVar = TrainColumnData[dependentVariable];
				cout<<"issueL2 \n";
				temp.independentVar = TrainColumnData[i];
				cout<<"issueL3 \n";
				temp.degree = j+1;
				cout<<"issueL4 \n";
				temp.runRegression();
				cout<<"issueL5 \n";
				temp.calculateMAE();
				cout<<"issueL6 \n";
				temp.calculateMSE();
				cout<<"issueL7 \n";
				temp.calculateRMSE();
				cout<<"issueL8 \n";
				tempRegressions.push_back(temp);
				cout<<"forLoopEnd \n";
				cout<<temp.rmse<<"rmse \n"<< temp.mse<<"mse \n"<< temp.mae<<"mae \n";
			}
			cout<<"here \n";
			cout<<tempRegressions[0].mse<<"\n";
			int index= 0;
			cout<<"here 3 \n";
			for(int i = 1 ; i< tempRegressions.size(); i++){
				if(tempRegressions[i].mse< tempRegressions[index].mse){
					index = tempRegressions[i].mse;
				}
			}
			cout<<"here2 \n"<<index<<"\n"<<tempRegressions.size()<<"\n";
			solutionVectors.push_back(tempRegressions[index].solutionVector);
			cout<<"heretrem \n";
			regressionData.push_back(tempRegressions[index]);
		}
	}
	cout<<"this \n"<< solutionVectors.size()<<"\n";
	for(int i = 0 ; i <solutionVectors.size()+1; i++){
		if(i != dependentVariable){
			cout<<"what is the value of "<<TrainColumnData[i].name<<"?\n";
			getline(cin, lineRead);
			double indepVal = stod(lineRead);
			double prediction;
			if(i<dependentVariable){
				prediction = regressionData[i].predict(indepVal);
				RMSEWH.predictions.push_back(prediction);
				MSEWH.predictions.push_back(prediction);
				MAEWH.predictions.push_back(prediction);
				cout<<prediction<<"\n";
				RMSEWH.weights.push_back(1/(regressionData[i].rmse));
				MSEWH.weights.push_back(1/(regressionData[i].mse));
				MAEWH.weights.push_back(1/(regressionData[i].mae));
			//	cout<<regressionData[0].rmse<<"rmse \n"<< regressionData[0].mse<<"mse \n"<< regressionData[0].mae<<"mae \n";
			}
			else{
				prediction = regressionData[i-1].predict(indepVal);
				RMSEWH.predictions.push_back(prediction);
				MSEWH.predictions.push_back(prediction);
				MAEWH.predictions.push_back(prediction);
				cout<<prediction<<"\n";
				RMSEWH.weights.push_back(1/(regressionData[i-1].rmse));
				MSEWH.weights.push_back(1/(regressionData[i-1].mse));
				MAEWH.weights.push_back(1/(regressionData[i-1].mae));
			}
		}
	}
	cout<<"your predicted value according to weighted RMSE is "<< RMSEWH.projection() <<"\n";
	cout<<"your predicted value according to weighted MSE is "<< MSEWH.projection() <<"\n";
	cout<<"your predicted value according to weighted MAE is "<< MAEWH.projection() <<"\n";


	return 0;
}
