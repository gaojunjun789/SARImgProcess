
#ifndef SAR_METHODS_H
#define SAR_METHODS_H

#include <vector>
#include "SARData.h"

struct SARDataType {
    static const unsigned int binarySARData = 0x01;
    static const unsigned int jsonSARData   = 0x02;
    static const unsigned int otherSARData  = 0x03;
};

struct Method {
    static const unsigned int meanAmplitude      = 0x01;
    static const unsigned int amplitudeNormalize = 0x02;
};

// -----------------------------------interface------------------------------
class SARMethods_if {
public:
    virtual void mean_amplitude()          = 0;
    virtual void amplitude_normalization() = 0;
};
// -----------------------------------implementation-------------------------
class binarySARMethods: public SARMethods_if {
public:
    binarySARMethods(std::vector<string> SARDataPath, unsigned int outType);
    ~binarySARMethods();
    void mean_amplitude();     
    void amplitude_normalization();   
private:
    void saveResult(string fileName);
	void saveJson(string fileName);
	void saveBinary(string fileName);
	void saveOther(string fileName);
    void outpuVisualization(string fileName);

    vector<binarySARData*> mSARData_ptr;
    unsigned int           mOutType;
	int32_t*               mResult;
    unsigned int           mRows;
    unsigned int           mCols;
};

class otherSARMethods: public SARMethods_if {
public:
    otherSARMethods();
    ~otherSARMethods();
    void mean_amplitude();     
    void amplitude_normalization();      
};

#endif
