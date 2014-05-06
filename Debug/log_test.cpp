#include "Log.h"

int main(int argc, char const *argv[])
{
	// Log *loger = Log::getInstance();
	// loger->writeLog(msg);
	std::string msg(" test msg ");
	WRITE_LOG(msg);
	return 0;
}