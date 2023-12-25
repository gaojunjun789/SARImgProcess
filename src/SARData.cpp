#include "SARData.h"
#include <math.h>
using namespace std;

//  ------------------------binarySARData-------------------
binarySARData::binarySARData(string SARFilePath) {
    // open SARData file
    mSARData_ifs.open (SARFilePath.c_str(),  ios::binary);
    bool result = mSARData_ifs.is_open();
    if (!result) {
        cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
        cout << "Failed to open the file. Check whether path " << SARFilePath << " exists" << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        return;
    }
    // Read the number of rows of the image
    char* data_ptr = new char[4];
    mSARData_ifs.read(data_ptr, 4);
    mRows = *(int32_t *)(data_ptr);
    // Read the number of cols of the image
    mSARData_ifs.read(data_ptr, 4);
    mCols = *(int32_t *)(data_ptr);
    delete[] data_ptr;
}
 
binarySARData::~binarySARData() {
    mSARData_ifs.close();
}

int binarySARData::getRealData(unsigned int rows, unsigned int cols) {
    // Boundary judgment
    if ((rows >= mRows) || (cols >= mCols)) {
        cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
        cout << "The number of rows and columns exceeds the maximum range." << endl;
        cout << "row is " << rows << ". Its maximum allowable value is " << mRows << endl;
        cout << "cols is " << cols << ". Its maximum allowable value is " << mCols << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        return -1;
    }
    // 8 is the length of the binary header.
    unsigned long long offset = (rows * mCols + cols) * sizeof(int32_t) + 8;
	//cout << "+++++ debug +++++ " << "offset is " << offset << ". rows is " << rows << ". cols" << cols << endl;
    mSARData_ifs.seekg(offset, ios::beg);
    // read dat 
    char* data_ptr = new char[4];
    mSARData_ifs.read(data_ptr, 4);
	//cout << "+++++ debug +++++ " << "data_ptr[0] is 0x" << std::hex << (int)data_ptr[0] << ". data_ptr[1] is 0x" << std::hex << (int)data_ptr[1] << endl;
	//cout << "+++++ debug +++++ " << "data_ptr[2] is 0x" << std::hex << (int)data_ptr[2] << ". data_ptr[3] is 0x" << std::hex << (int)data_ptr[3] << endl;
	int32_t value = *(int32_t*)(data_ptr);
    delete[] data_ptr;
    return value;
}

int binarySARData::getImaginaryData(unsigned int rows, unsigned int cols) {
    // Boundary judgment
    if ((rows >= mRows) || (cols >= mCols)) {
        cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
        cout << "The number of rows and columns exceeds the maximum range." << endl;
        cout << "rows is " << rows << ". Its maximum allowable value is " << mRows << endl;
        cout << "cols is " << cols << ". Its maximum allowable value is " << mCols << endl;
		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        return -1;
    }
    // 8 is the length of the binary header. 4 is the real part of the pixel
    unsigned long long offset = (rows * mCols + cols) * sizeof(int32_t) + 8 + 4;
	//cout << "+++++ debug +++++ " << "offset is " << offset << ". rows is " << rows << ". cols" << cols << endl;
    mSARData_ifs.seekg(offset, ios::beg);
    // read dat 
    char* data_ptr = new char[4];
    mSARData_ifs.read(data_ptr, 4);
    int value = *(int32_t*)(data_ptr);
    delete[] data_ptr;
    return value;
}

int binarySARData::getAmplitudeData(unsigned int rows, unsigned int cols) {
    // Get real and imaginary part data
    int re = getRealData(rows, cols);
    int im = getImaginaryData(rows, cols);
    // Calculate amplitude
    int Amplitude = sqrt(pow(re, 2) + pow(im, 2));
	//cout << "+++++ debug +++++ " << "re is " << re << ". im is " << im << ". Amplitude" << Amplitude << endl;
    return Amplitude;
}

//  ------------------------implementation---------------------
otherSARData::otherSARData() {
    
}
 
otherSARData::~otherSARData() {
    
}

int otherSARData::getRealData(unsigned int rows, unsigned int cols) {
    return 0;
}

int otherSARData::getImaginaryData(unsigned int rows, unsigned int cols) {
    return 0;
}

int otherSARData::getAmplitudeData(unsigned int rows, unsigned int cols) {
    return 0;
}
