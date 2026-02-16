#include "escape_from_the_island.hpp"

//Name of the file containing all the neccessary config info
#define INI_FILE_NAME "config.ini"s

//All posible data type that a selector can hold
enum SelectorDataType : uint64_t
{
	NUMBER = 0,
	BOOL = 1,
	TEXT = 2
};

//Reads all the file content by lines and puts it into a array of lines
static void ExtractFileIntoLines(std::vector<std::string>& _FileLines)
{
	std::string OneLine;
	std::fstream FileHandle(SDL_GetBasePath() + INI_FILE_NAME, std::ios::in);

	if (FileHandle.fail())
		ErrorHandle::Report("could not find/open config file {\"config.ini\"}", -1, true);

	_FileLines.clear();

	while (!FileHandle.eof())
	{
		std::getline(FileHandle, OneLine);
		_FileLines.push_back(OneLine);
	}

	FileHandle.close();

	return;
};

//Truncates all current content it the file and replaces it with the new lines from the array
static void OverwriteFileWithNewContent(std::vector<std::string>& _NewFileLines)
{
	std::fstream FileHandle(SDL_GetBasePath() + INI_FILE_NAME, std::ios::out | std::ios::trunc);

	if (FileHandle.fail())
		ErrorHandle::Report("could not find/open config file {\"config.ini\"}", -1, true);

	for (std::string OneLine : _NewFileLines)
		FileHandle.write((OneLine + "\n").c_str(), OneLine.length() + 1);

	FileHandle.close();

	return;
};

//Finds the index of a line that only contains the specified bundle
static uint64_t FindLineWithBundle(const std::vector<std::string>& _FileLines, const std::string& _Bundle)
{
	for (uint64_t c = NULL; c < _FileLines.size(); c++)
		if (_FileLines[c] == "[" + _Bundle + "]")
			return c;

	ErrorHandle::Report("could not find the bundle {" + _Bundle + "} in the given config file", -1, true);

	return NULL;
};

//Verifies if the specified line is an actuall bundle with at least one char long name
static INLINE bool IsBundle(const std::string& _OneLine)
{
	if (_OneLine.length() < 3)
		return false;

	return (_OneLine[0] == '[' && _OneLine[_OneLine.length() - 1] == ']');
};

//Verifies if the specified line is a specified selector with at least one char long value
static INLINE bool IsSpecificSelectorWithValue(const std::string& _OneLine, const std::string& _Selector)
{
	if (_OneLine.length() < _Selector.length() + 2)
		return false;

	return _OneLine.contains(_Selector + "=");
};

//Finds the index of a line that only contains the specified selector and its within the specified bundle sector
static uint64_t FindLineWithSelector(const std::vector<std::string>& _FileLines, const uint64_t _BundleSectorStart, const std::string& _Selector)
{
	for (uint64_t c = _BundleSectorStart + 1; c < _FileLines.size(); c++)
	{
		if (IsBundle(_FileLines[c]))
			break;

		if (IsSpecificSelectorWithValue(_FileLines[c], _Selector))
			return c;
	}

	ErrorHandle::Report("could not find the selector {" + _Selector + "} in the given bundle range in the config file", -1, true);

	return NULL;
};

//Gets the basic informations about the location of the value in the file
static INLINE void ExtractBasicInfo(const std::string& _Bundle, const std::string& _Selector, const std::vector<std::string>& _ExtractedFileLines, uint64_t& _ExtractedSelectorPosition, uint64_t& _ExtractedEqualSignPosition)
{
	uint64_t BundleSectorStart = FindLineWithBundle(_ExtractedFileLines, _Bundle);
	_ExtractedSelectorPosition = FindLineWithSelector(_ExtractedFileLines, BundleSectorStart, _Selector);
	_ExtractedEqualSignPosition = _ExtractedFileLines[_ExtractedSelectorPosition].find('=');

	return;
};

//Returns a numberic value based on selector's first char value
static INLINE SelectorDataType TellSelectorDataType(const std::string& _SpecificSelector)
{
	//Anything else than char 'b' and 't' is considerer as number
	return
		_SpecificSelector[0] == 'b'	? BOOL :
		_SpecificSelector[0] == 't'	? TEXT :
		NUMBER;
};

namespace ConfigFile //[start]
{

//Finds the value based on specific Bundle and Selector and extracts it as Number type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, int64_t& _ExtractedValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != NUMBER)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	_ExtractedValue = //Interpret as number
		std::stoll(FileLines[SelectorPosition].substr(EqualSignPosition + 1));

	return;
};

//Finds the value based on specific Bundle and Selector and extracts it as Bool type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, bool& _ExtractedValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != BOOL)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	_ExtractedValue = //Interpret as bool
		(FileLines[SelectorPosition].substr(EqualSignPosition + 1) == "true");

	return;
};

//Finds the value based on specific Bundle and Selector and extracts it as Text type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, std::string& _ExtractedValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != TEXT)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	_ExtractedValue = //Interpret as text
		FileLines[SelectorPosition].substr(EqualSignPosition + 1);

	return;
};

//Finds the old value based on specific Bundle and Selector and changes it to the new Number type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const int64_t& _NewValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != NUMBER)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	//Overwrite with new number
	FileLines[SelectorPosition] = FileLines[SelectorPosition].substr(0, EqualSignPosition + 1) + std::to_string(_NewValue);
	OverwriteFileWithNewContent(FileLines);

	return;
};

//Finds the old value based on specific Bundle and Selector and changes it to the new Bool type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const bool& _NewValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != BOOL)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	//Overwrite with new bool
	FileLines[SelectorPosition] = FileLines[SelectorPosition].substr(0, EqualSignPosition + 1) + (_NewValue ? "true" : "false");
	OverwriteFileWithNewContent(FileLines);

	return;
};

//Finds the old value based on specific Bundle and Selector and changes it to the new Text type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const std::string& _NewValue)
{
	std::vector<std::string> FileLines;
	uint64_t SelectorPosition = NULL;
	uint64_t EqualSignPosition = NULL;

	ExtractFileIntoLines(FileLines);
	ExtractBasicInfo(_Bundle, _Selector, FileLines, SelectorPosition, EqualSignPosition);

	if (TellSelectorDataType(FileLines[SelectorPosition]) != TEXT)
		ErrorHandle::Report("Type mismatch in value at bundle {" + _Bundle + "}, selector {" + _Selector + "}", -1, true);

	//Overwrite with new text
	FileLines[SelectorPosition] = FileLines[SelectorPosition].substr(0, EqualSignPosition + 1) + _NewValue;
	OverwriteFileWithNewContent(FileLines);

	return;
};

}
//ConfigFile [end]