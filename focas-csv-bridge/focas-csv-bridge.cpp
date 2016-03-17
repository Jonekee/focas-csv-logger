// focas-csv-bridge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "30i\fwlib32.h"
#include "focas-csv-bridge.h"
#include "math.h"

int main(int argc, char *argv[])
{
	int result = -1;
	unsigned short fwlibHndl;
	bool mConnected = false;
	short mQtyAxis = 0;
	short mQtySpindle = 0;
	short mQtyPath = 0;

	printf("FANUC Logger v0.1\n");
	printf("=======================\n");

	// ************* Connection *****************
	printf("\nConnection\n");
	printf("-----------------------\n");
	result = cnc_allclibhndl3("192.168.82.16", 8193, 10, &fwlibHndl);
	if (result == 0) { mConnected = true; }
	printf("Result of cnc_allclibhndl3 is: %d\n", result);


	// ******* Number of Axis and Spindle *******
	if (mConnected){
		ODBSYSEX sysinfo;
		result = cnc_sysinfo_ex(fwlibHndl, &sysinfo);
		if (result == 0)
		{ 
			mQtyAxis = sysinfo.ctrl_axis;
			mQtySpindle = sysinfo.ctrl_spdl;
			mQtyPath = sysinfo.ctrl_path;
		}
		printf("The number of axis is %d\n", mQtyAxis);
	}

	// *************** Retrieve Data ***********
	getDiagnositics(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);
	getParameters(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);


	// ************* Disconnection *****************
	printf("\nDisconnection\n");
	printf("-----------------------\n");
	result = cnc_freelibhndl(fwlibHndl);
	if (result == 0) { mConnected = true; }
	printf("Result of cnc_freelibhndl is: %d\n", result);

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