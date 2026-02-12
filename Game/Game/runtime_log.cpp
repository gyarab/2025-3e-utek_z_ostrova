#include "escape_from_the_island.hpp"

//Container containing the name of the log file
struct LogFileAssets
{
	static std::string filename;
};

std::string LogFileAssets::filename = "";

//Gets current time as a number of seconds since epoch converted to string
static INLINE std::string CurrentTimeStamp(void)
{
	return 
		std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
};

//Gets current time with local timezone applied as a number of seconds
static INLINE std::chrono::local_seconds CurrentLocalTimeInSeconds(void)
{
	return
		std::chrono::current_zone()->to_local(std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
};

//Gets current time and formats it into a D-M-Y H:M:S string
static INLINE std::string CurrentDateTimeString(void)
{
	return std::format("{:%d-%m-%Y %H:%M:%S}", CurrentLocalTimeInSeconds());
};

//Gets current time and formats it into a H:M:S string
static INLINE std::string CurrentTimeString(void)
{
	return std::format("{:%T}", CurrentLocalTimeInSeconds());
};

//Changes the given log type to a string that can be written to log file
static INLINE std::string LogTypeToString(const LogTypes _Type)
{
	return
		_Type == INFO		? " [INFO] " :
		_Type == WARNING	? " [WARNING] " :
		_Type == ERROR		? " [ERROR] " :
		_Type == CRASH		? " [CRASH] " :
		"";
};

namespace RuntimeLog //[start]
{

//Creates a new file for log messages - needs to be called first
void CreateFile(void)
{
	//Creates a new directory for the logs - if it already exists, nothing will happen
	std::filesystem::create_directory((std::string)SDL_GetBasePath() + "logs");

	LogFileAssets Assets;
	Assets.filename = (std::string)SDL_GetBasePath() + "logs\\" + CurrentTimeStamp() + "-runtime.log";
	std::string FirstMessage = CurrentDateTimeString() + " [INFO] log created \n";

	std::fstream LogFile(Assets.filename, std::ios::out | std::ios::trunc);
	LogFile.write(FirstMessage.c_str(), FirstMessage.size());
	LogFile.close();

	return;
};

//Appends a new message to already existing log file - usage without existing log file may be unsafe
void Message(const LogTypes _Type, const std::string& _Message)
{
	LogFileAssets Assets;
	std::string MessageToLog = CurrentDateTimeString() + LogTypeToString(_Type) + _Message + "\n";

	std::fstream LogFile(Assets.filename, std::ios::out | std::ios::app);
	LogFile.write(MessageToLog.c_str(), MessageToLog.size());
	LogFile.close();

	return;
};

}
//RuntimeLog [end]