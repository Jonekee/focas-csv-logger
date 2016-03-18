// focas-csv-bridge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "30i\fwlib32.h"
#include "focas-csv-bridge.h"
#include "math.h"
#include "minIni.h"

long diagnostics[32];
long parameters[32];
struct LogLine diagLogLine[32];
struct LogLine paramLogLine[32];
struct LogLine modeLogLine;
struct AlarmLine alarmLogLine[4];
int numOfDiagnostics = 0;
int numOfParameters = 0;

int main(int argc, char *argv[])
{
	// ********** Parse Command Line Arguments ************
	char* listFilename = "";
	char* ipAddress = "192.168.82.16";
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
	{
		parameters[k] = atoi(key);
		numOfParameters += 1;
	}
		
	for (int k = 0; ini_getkey("diagnostics", k, key, 20, listFilename) > 0; k++)
	{
		diagnostics[k] = atoi(key);
		numOfDiagnostics += 1;
	}
		


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
	getAlarms(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);
	getMode(fwlibHndl, mQtyPath, mQtyAxis, mQtySpindle);


	// ************* Disconnection *****************
	result = cnc_freelibhndl(fwlibHndl);
	if (result == 0) { mConnected = true; }

	return 0;
}

void getDiagnositics(unsigned short fwlibHndl, short mQtyPath, short mQtyAxis, short mQtySpindle)
{
	// ************** Diagnostics ***************

		IODBPRM param[2];
		int dgnType = -1;
		int loopCount = 0;
		int result = -1;

		result = cnc_rddiag_ext(fwlibHndl, diagnostics, numOfDiagnostics, param);
		if (result == 0){
			for (int i = 0; i < numOfDiagnostics; i++){
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
	IODBPRM param[2];
	int paramType = -1;
	int loopCount = 0;
	int result = -1;

	result = cnc_rdparam_ext(fwlibHndl, parameters, numOfParameters, param);
	if (result == 0){
		for (int i = 0; i < numOfParameters; i++){
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

void getAlarms(unsigned short fwlibHndl, short mQtyPath, short mQtyAxis, short mQtySpindle)
{

	// ************** Alarms ***************
	ODBALMMSG2 almmsg[4];
	short alarmType = -1;
	short numOfAlarms = 4;
	int result = -1;
	char message[128];

	result = cnc_rdalmmsg2(fwlibHndl, alarmType, &numOfAlarms, almmsg);
	if (result == 0){
		for (int i = 0;i<4; i++)
		{
			if (i < numOfAlarms){
				sprintf(message, "%d %s", almmsg[i].alm_no, almmsg[i].alm_msg);
				strcpy(alarmLogLine[i].currentValue, message );
				alarmLogLine[i].currentType = almmsg[i].type;
			}
			else {
				strcpy(alarmLogLine[i].currentValue, "");
				alarmLogLine[i].currentType = -1;
			}
			
		}
	}
	return;
}

void getMode(unsigned short fwlibHndl, short mQtyPath, short mQtyAxis, short mQtySpindle)
{
	int result = -1;
	ODBST2 statinfo;

	result = cnc_statinfo2(fwlibHndl, &statinfo);
	if (result == 0){
		strcpy(modeLogLine.key, "MODE");
		switch (statinfo.aut)
		{
		case 0: // MDI
			break;
		case 1: // MEMory
			break;
		case 2: // ****
			break;
		case 3: // EDIT
			break;
		case 4: // HaNDle
			break;
		case 5: // JOG
			break;
		case 6: // Teach in JOG
			break;
		case 7: // Teach in HaNDle
			break;
		case 8: // INC
			break;
		case 9: // REFerence
			break;
		case 10: // ReMoTe
			break;
		}
	}
	return;
}