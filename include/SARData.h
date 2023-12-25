
#ifndef SARDATA_H
#define SARDATA_H

#include <vector>
#include "iostream"
#include "fstream"
#include <string>
#include <iostream>

using namespace std;

// -----------------------------------interface------------------------------
class SARData_if {
public:
    // Obtain the imaginary data of the specified coordinate pixels
    virtual int  getRealData(unsigned int rows, unsigned int cols) = 0;
    // Obtain the real data of the specified coordinate pixels
    virtual int  getImaginaryData(unsigned int rows, unsigned int cols) = 0;
    // Obtain the Amplitude data of the specified coordinate pixels
    virtual int  getAmplitudeData(unsigned int rows, unsigned int cols) = 0;
};

// -----------------------------------binarySARData implementation-------------------------
class binarySARData: public SARData_if {
public:
    binarySARData(string SARFilePath);
    ~binarySARData();
    // implement SARData_if
    int  getRealData(unsigned int rows, unsigned int cols);
    int  getImaginaryData(unsigned int rows, unsigned int cols);
    int  getAmplitudeData(unsigned int rows, unsigned int cols);
    // get image cols and rows
    unsigned int getImgCols() {return mCols;}
    unsigned int getImgRows() {return mRows;}
private:
    ifstream     mSARData_ifs;
    unsigned int mRows;
    unsigned int mCols;
     
};

// -----------------------------------otherSARData implementation-------------------------
class otherSARData: public SARData_if {
public:
    otherSARData();
    ~otherSARData();
    int  getRealData(unsigned int rows, unsigned int cols);
    int  getImaginaryData(unsigned int rows, unsigned int cols);
    int  getAmplitudeData(unsigned int rows, unsigned int cols);
private:
    ifstream SARData_ifs;
    unsigned int mRows;
    unsigned int mCols;
};

#endif
