#ifndef CONFIG_FILE_CONTENT_INFO
#define CONFIG_FILE_CONTENT_INFO

//All available bundles to use
#define BUNDLE_AppWindowConfig "AppWindowConfig"
#define BUNDLE_AppConfig "AppConfig"
#define BUNDLE_AppAssetConfig "AppAssetConfig"
#define BUNDLE_AppEntityConfig "AppEntityConfig"

//All available selectors to use
#define SELECTOR_Width "Width"
#define SELECTOR_Length "Length"
#define SELECTOR_ScalingCoefficient "ScalingCoefficient"
#define SELECTOR_tWindowTitle "tWindowTitle"
#define SELECTOR_fpsCap "FpsCap"
#define SELECTOR_tAppDir "tAppDir"
#define SELECTOR_bRuntimeLogEnabled "bRuntimeLogEnabled"
#define SELECTOR_tLogDir "tLogDir"
#define SELECTOR_MaxLogFiles "MaxLogFiles"
#define SELECTOR_tGraphicsDir "tGraphicsDir"
#define SELECTOR_tTextureFormat "tTextureFormat"
#define SELECTOR_tAssetConfigDir "tAssetConfigDir"
#define SELECTOR_tPlayerTextures "tPlayerTextures"
#define SELECTOR_tBackgroundTextures "tBackgroundTextures"
#define SELECTOR_tMenuPanelTextures "tMenuPanelTextures"
#define SELECTOR_tObstaclesTextures "tObstaclesTextures"
#define SELECTOR_tPlayer "tPlayer"
#define SELECTOR_tBackground "tBackground"
#define SELECTOR_tBackgroundHitbox "tBackgroundHitbox"
#define SELECTOR_tMenuPanel "tMenuPanel"
#define SELECTOR_tMenuButtons "tMenuButtons"
#define SELECTOR_tFloatingStone "tFloatingStone"
#define SELECTOR_tPoisonedArrow "tPoisonedArrow"
#define SELECTOR_tCrate "tCrate"

//This is a concept its not meant to be compiled!
#if 0
struct config
{
	//[AppWindowConfig]
	uint64_t _Width;
	uint64_t _Length;
	uint64_t _ScalingCoefficient;
	std::string _tWindowTitle;
	uint64_t _FpsCap;

	//[AppConfig]
	std::string _tAppDir;
	bool _bRuntimeLogEnabled;
	std::string _tLogDir;
	uint64_t _MaxLogFiles;

	//[AppAssetConfig]
	std::string _tGraphicsDir; 
	std::string _tTextureFormat; 
	std::string _tAssetConfigDir; 
	std::string _tPlayerTextures;
	std::string _tBackgroundTextures;
	std::string _tMenuPanelTextures;
	std::string _tObstaclesTextures;

	//[AppEntityConfig]
	std::string _tPlayer; 
	std::string _tBackground; 
	std::string _tBackgroundHitbox; 
	std::string _tMenuPanel; 
	std::string _tMenuButtons; 
	std::string _tFloatingStone; 
	std::string _tPoisonedArrow; 
	std::string _tStone;
};
#endif

#endif