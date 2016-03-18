// focas-csv-bridge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "30i\fwlib32.h"
#include "focas-csv-bridge.h"
#include "math.h"
#include "minIni.h"

int main(int argc, char *argv[])
{
	// ********** Parse Command Line Arguments ************
	char* listFilename = "";
	char* ipAddress = "192.168.1.1";
	unsigned short port = 8193;

	if (argc < 3) 
	{
		printf("Usage is [application] -f <listOfData> -i <ipAddress> -p <port>\n");
		return 0;
	}
	else
	{
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc)
			{
				if (strcmp(argv[i], "-f") == 0) { listFilename = argv[i + 1]; }
				else if (strcmp(argv[i], "-i") == 0) { ipAddress = argv[i + 1]; }
				else if (strcmp(argv[i], "-p") == 0) { port = atoi(argv[i + 1]); }
			}
		}
	}

	// Load Parameters and Diagnostics from ini File
	char key[20];
	for (int k = 0; ini_getkey("parameters", k, key, 20, listFilename) > 0; k++)
		printf("%s\n", key);
	for (int k = 0; ini_getkey("diagnostics", k, key, 20, listFilename) > 0; k++)
		printf("%s\n", key);


	// ************* Connection *****************
	int result = -1;
	unsigned short fwlibHndl;
	bool mConnected = false;

	result = cnc_allclibhndl3(ipAddress, port, 10, &fwlibHndl);
	if (result == 0) { mConnected = true; }


	// ******* Number of Axis and Spindle *******
	short mQtyAxis = 0;
	short mQtySpindle = 0;
	short mQtyPath = 0;

	if (mConnected){
		ODBSYSEX sysinfo;
		result = cnc_sysinfo_ex(fwlibHndl, &sysinfo);
		if (result == 0)
		{ 
			mQtyAxis = sysinfo.ctrl_axis;
			mQtySpindle = sysinfo.ctrl_spdl;
			mQtyPath = sysinfo.ctrl_path;
		}
	}

	// *************** Retrieve Data ***********
	getDiagnositics(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);
	getParameters(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);


	// ************* Disconnection *****************
	result = cnc_freelibhndl(fwlibHndl);
	if (result == 0) { mConnected = true; }

	return 0;
}

void getDiagnositics(unsigned short fwlibHndl, short mQtyPath, short mQtyAxis, short mQtySpindle)
{
	// ************** Diagnostics ***************
		long dgns[2];
		short qtyToRead = 2;
		IODBPRM param[2];
		int dgnType = -1;
		int loopCount = 0;
		int result = -1;

		dgns[0] = 301;
		dgns[1] = 302;
		result = cnc_rddiag_ext(fwlibHndl, dgns, qtyToRead, param);
		if (result == 0){
			for (int i = 0; i < qtyToRead; i++){
				dgnType = param[i].type;
				if ((param[i].axis & 0x03) == 0x01){ loopCount = mQtyAxis; }
				else if ((param[i].axis & 0x03) == 0x03){ loopCount = mQtySpindle; }
				else { loopCount = 1; }
				for (int j = 0; j < loopCount; j++)
				{
					if (dgnType == -1){}
					else if (dgnType == 0)
					{
						printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
					}
					else if (dgnType == 1)
					{
						printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
					}
					else if (dgnType == 2)
					{
						printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
					}
					else if (dgnType == 3)
					{
						printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
					}
					else if (dgnType == 4){}
					else if (dgnType == 5)
					{
						double denominator = pow((double)10, (param[i].data[j].dec_val));
						int numerator = param[i].data[j].prm_val;
						double quotient = numerator / denominator;
						printf("1%05d%d : %.3f\n", param[i].datano, j, quotient);
					}
				}
			}
		}
		return;

}


void getParameters(unsigned short fwlibHndl, short mQtyPath, short mQtyAxis, short mQtySpindle)
{
	// ************** Parameters ***************
	long params[2];
	short qtyToRead = 2;
	IODBPRM param[2];
	int paramType = -1;
	int loopCount = 0;
	int result = -1;

	params[0] = 20;
	params[1] = 3202;
	result = cnc_rdparam_ext(fwlibHndl, params, qtyToRead, param);
	if (result == 0){
		for (int i = 0; i < qtyToRead; i++){
			paramType = param[i].type;
			if ((param[i].axis & 0x03) == 0x01){ loopCount = mQtyAxis; }
			else if ((param[i].axis & 0x03) == 0x03){ loopCount = mQtySpindle; }
			else { loopCount = 1; }
			for (int j = 0; j < loopCount; j++)
			{
				if (paramType == -1){}
				else if (paramType == 0)
				{
					printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
				}
				else if (paramType == 1)
				{
					printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
				}
				else if (paramType == 2)
				{
					printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
				}
				else if (paramType == 3)
				{
					printf("1%05d%d : %d\n", param[i].datano, j, param[i].data[j].prm_val);
				}
				else if (paramType == 4){}
				else if (paramType == 5)
				{
					double denominator = pow((double)10, (param[i].data[j].dec_val));
					int numerator = param[i].data[j].prm_val;
					double quotient = numerator / denominator;
					printf("1%05d%d : %.3f\n", param[i].datano, j, quotient);
				}
			}
		}
	}
	return;

}