
struct LogLine {
	char key[128];
	char currentValue[128];
	char previousValue[128];
	time_t lastChange;
};

struct AlarmLine{
	char currentValue[128];
	short currentType;
	char previousValue[128];
	short previousType;
	time_t lastChange;
};

void getDiagnositics(unsigned short, short, short, short);
void getParameters(unsigned short, short, short, short);
void getAlarms(unsigned short, short, short, short);
void getMode(unsigned short, short, short, short);
