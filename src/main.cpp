#include <iostream>
#include <string.h>
#include "SARMethods.h"
#include "SARData.h"

using namespace std;

struct config{
    unsigned int   inputImageType  = SARDataType::binarySARData;
    unsigned int   outputImageType = SARDataType::jsonSARData;
    unsigned int   method          = Method::meanAmplitude;
    vector<string> inputImagePath;
};

void parseParameter(int argc, char **argv, config& params);

int main(int argc, char **argv) {
    config params;
    parseParameter(argc, argv, params);

    SARMethods_if* mehtod;
    switch (params.inputImageType) {
        case SARDataType::binarySARData:
            mehtod = new binarySARMethods(params.inputImagePath, params.outputImageType);
			cout << "+++++ debug +++++ " << "params.outputImageType is " << params.outputImageType << endl;
            break;
        case SARDataType::otherSARData:
            mehtod = new otherSARMethods();
            break;
        default:
            cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
            cout << "This SAR data format is not supported." << endl;
            cout << "SAR data type id is " << params.inputImageType << endl;
            cout << "The expected SAR data type id is 0x01 or 0x02." << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            return 0;
    }

    switch (params.method) {
        case Method::meanAmplitude:
            mehtod->mean_amplitude();
            break;
        case Method::amplitudeNormalize:
            mehtod->amplitude_normalization();
            break;
        default:
            cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
            cout << "This method of processing is not supported." << endl;
            cout << "SAR method id is " << params.method << endl;
            cout << "The expected SAR method id is 0x01 or 0x02." << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            return 0;
    }
    
}

void parseParameter(int argc, char **argv, config& params) {
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
			unsigned int val = static_cast<unsigned int>(atoi(argv[++i]));
			cout << "+++++ debug +++++ -i val is " << val << endl;
            params.inputImageType = val;
        } else if (strcmp(argv[i], "-o") == 0) {
			unsigned int val = static_cast<unsigned int>(atoi(argv[++i]));
			cout << "+++++ debug +++++ -o val is " << val << endl;
			params.outputImageType = val;
        } else if (strcmp(argv[i], "-m") == 0) {
			unsigned int val = static_cast<unsigned int>(atoi(argv[++i]));
			cout << "+++++ debug +++++ -m val is " << val << endl;
			params.method = val;
        }else if (strcmp(argv[i], "-f") == 0) {
            ifstream infile;
            infile.open(argv[++i], ios::in);
            if (!infile.is_open()) {
                cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
                cout << "The file failed to open." << endl;
                cout << "Please check whether the file exists." << params.inputImageType << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                return;
            }
            string path;
            while (getline(infile, path)) {
				cout << "+++++ debug +++++ -f path is " << path << endl;
                params.inputImagePath.push_back(path);
            }
            infile.close();
        }
    }
}

//int main( ) {
//	cout << "---------------------------" << endl;
//	ofstream mSARData_ofs;
//	mSARData_ofs.open("F:\\SARProj\\test\\build-vs\\Release\\data_file\\test0.bin", ios::binary);
//	bool result = mSARData_ofs.is_open();
//	if (!result) {
//		cout << "++++++++++++++++++++++ error log ++++++++++++++++++++++" << endl;
//		cout << "Failed to open the file. Check whether path exists" << endl;
//		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
//		return 0;
//	}
//	int32_t* buffer = new int32_t[(512 * 512 + 1) * 2];
//	buffer[0] = 512;
//	buffer[1] = 512;
//	for (int i = 2; i < (512 * 512 ) * 2; i++) {
//		buffer[i] = 0;
//	}
//	mSARData_ofs.write((char*)buffer, sizeof(int32_t)*(512 * 512 + 1) * 2);
//	mSARData_ofs.close();
//	cout << "++++++++++++++++++++++++" << endl;
//	delete[] buffer;
//
//	return 0;
//}
