#include "SARMethods.h"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>

#include "json.h"

using namespace std;

//  ------------------------binarySARData-------------------
binarySARMethods::binarySARMethods(vector<string> SARDataPath, unsigned int outType) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::binarySARMethods()" << endl;
	mOutType = outType;
	cout << "+++++ debug +++++ " << "mOutType is " << mOutType << endl;
	cout << "+++++ debug +++++ " << "outType is " << outType << endl;
    if (SARDataPath.size() == 0) {
        cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
        cout << "No SAR image data." << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        return ;
    }
    for(int i = 0; i < SARDataPath.size(); i++) {
         binarySARData* ptr = new binarySARData(SARDataPath[i]);
         mSARData_ptr.push_back(ptr);
    }
    mRows = mSARData_ptr[0]->getImgRows();
    mCols = mSARData_ptr[0]->getImgCols();
	
    for(int i = 0; i < mSARData_ptr.size(); i++) {
        if ((mRows != mSARData_ptr[i]->getImgRows()) ||
            (mCols != mSARData_ptr[i]->getImgCols())) {
                cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
                cout << "The size of the loaded image is different." << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                return ;
        }
    }
    mResult = new int32_t[mRows * mCols];
}

binarySARMethods::~binarySARMethods() {
	cout << "+++++ debug enter +++++" << "binarySARMethods::~binarySARMethods()" << endl;
    for (int i = 0; i < mSARData_ptr.size(); i++) {
        delete mSARData_ptr[i];
    }
    delete[] mResult;
}

void binarySARMethods::mean_amplitude() {
    // Go through each pixel, get the amplitude value of all the images at that point,
    // and then calculate the average.
	cout << "+++++ debug enter +++++" << "binarySARMethods::mean_amplitude()" << endl;

    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
			//cout << "+++++ debug +++++ " << "i is " << i << " . j is " << j << endl;
            for (unsigned int k = 0; k < mSARData_ptr.size(); k++) {
                unsigned int amplitude = mSARData_ptr[k]->getAmplitudeData(i, j);
				//cout << "+++++ debug +++++ " << "amplitude is " << amplitude << endl;
                mResult[i * mCols + j] += amplitude / mSARData_ptr.size();
				//cout << "+++++ debug +++++ " << "mSARData_ptr.size() is " << mSARData_ptr.size() << endl;
				//cout << "+++++ debug +++++ " << "mResult[i * mCols + j] is " << mResult[i * mCols + j] << endl;
            }
        }
    }
	saveResult("meanAmplitude");
	outpuVisualization("meanAmplitudeVisableResult");
}

void binarySARMethods::amplitude_normalization() {
	cout << "+++++ debug enter +++++" << "binarySARMethods::amplitude_normalization()" << endl;
    // get amplitude mean
    mean_amplitude();
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            double variance = 0;
            // s²=((x1-u)^2 +(x2-u)^2 +...+(xn-u)^2)/n
            for (int k = 0; k < mSARData_ptr.size(); k++) {
                double amplitude = mSARData_ptr[k]->getAmplitudeData(i, j);
                double mean_ampl = mResult[i * mCols + j];
                variance += pow((amplitude - mean_ampl), 2);
            }
            // σ=sqrt(s²) 
            mResult[i * mCols + j] = sqrt(variance);
        }
    }
    saveResult("amplitudeNormalizeRst");
    outpuVisualization("amplitudeNormalizeVisableRst");
}

void binarySARMethods::saveResult(string fileName) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::saveResult()" << endl;
    switch (mOutType) {
        case SARDataType::binarySARData:
            saveBinary(fileName);
            break;
        case SARDataType::jsonSARData:
            saveJson(fileName);
            break;
        case SARDataType::otherSARData:
            saveOther(fileName);
            break;
    }
}

void binarySARMethods::saveJson(string fileName) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::saveJson()" << endl;
    // Root node
	Json::Value root;
	root["author"] = Json::Value("Junjun Gao");
	root["age"] = Json::Value(26);
	root["sex"] = Json::Value("female");
    root["rows"] = Json::Value(mRows);
    root["cols"] = Json::Value(mCols);
    // Child node
	
	Json::Value  Data;
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
			// cout << "+++++ debug +++++" << " i is " << i << ". j is " << j << endl;
			string key = "data [" + to_string(i) + "][" + to_string(j) + "]";
            Data[key.c_str()]  = Json::Value(mResult[i * mCols + j]);
        }
    }
	root["Data"] = Json::Value(Data);
	Json::StyledWriter sw;
	//save json file  
	ofstream os;
	os.open((".\\data_file\\" + fileName + "_jsonResult.json").c_str(), std::ios::out | std::ios::app);
	if (!os.is_open())
		cout << "error: can not find or create the file which named ./../additon/" << fileName << ".json" << endl;
	os << sw.write(root);
	os.close();
	

}

void binarySARMethods::saveBinary(string fileName) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::saveBinary()" << endl;
	ofstream ofs;
	ofs.open((".\\data_file\\" + fileName + "_binaryResult.bin").c_str(), ios::out);
	if (!ofs.is_open()) {
		cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
		cout << "The out file failed to open." << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		return;
	}
	//cout << "+++++ debug +++++" << "sizeof(int32_t) * 2 is " << sizeof(int32_t) * 2 << endl;
	int32_t size[2] = {mRows, mCols};
	ofs.write((char*)size, sizeof(int32_t) * 2);
	ofs.write((char*)mResult, sizeof(int32_t) * (mRows * mCols));
	ofs.close();
}

void binarySARMethods::saveOther(string fileName) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::saveOther()" << endl;
}

void binarySARMethods::outpuVisualization(string fileName) {
	cout << "+++++ debug enter +++++" << "binarySARMethods::outpuVisualization()" << endl;
	cv::Mat img = cv::Mat::zeros(mRows, mCols, CV_8UC1);
	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mCols; j++) {
			img.data[i * mCols + j] = mResult[i * mCols + j] / 0xff;
		}
	}
	string path = ".\\data_file\\" + fileName + "_VisuaResult.jpg";
    imwrite(path.c_str(), img);
}

//  ------------------------implementation---------------------
otherSARMethods::otherSARMethods() {
   
}
 
otherSARMethods::~otherSARMethods() {
    
}

void otherSARMethods::mean_amplitude() {

}

void otherSARMethods::amplitude_normalization() {

}
