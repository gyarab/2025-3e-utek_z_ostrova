#ifndef CONFIG_FILE_CONTENT_INFO
#define CONFIG_FILE_CONTENT_INFO

//All Available bundles to use
#define BUNDLE_AppWindowConfig "AppWindowConfig"
#define BUNDLE_AppConfig "AppConfig"
#define BUNDLE_AppAssetConfig "AppAssetConfig"

//All Available selectors to use
#define SELECTOR_Width "Width"
#define SELECTOR_Length "Length"
#define SELECTOR_tRatio "tRatio"
#define SELECTOR_TextureScalingCoefficient "TextureScalingCoefficient"
#define SELECTOR_tWindowTitle "tWindowTitle"
#define SELECTOR_fpsCap "FpsCap"
#define SELECTOR_tAppDir "tAppDir"
#define SELECTOR_bRuntimeLogEnabled "bRuntimeLogEnabled"
#define SELECTOR_tLogDir "tLogDir"
#define SELECTOR_MaxLogFiles "MaxLogFiles"
#define SELECTOR_tGraphicsDir "tGraphicsDir"
#define SELECTOR_tTextureFormat "tTextureFormat"
#define SELECTOR_tPlayerTextures "tPlayerTextures"
#define SELECTOR_PlayerTexturesWidth "PlayerTexturesWidth"
#define SELECTOR_PlayerTexturesLength "PlayerTexturesLength"
#define SELECTOR_tLevel1Textures "tLevel1Textures"
#define SELECTOR_Level1TexturesWidth "Level1TexturesWidth"
#define SELECTOR_Level1TexturesLength "Level1TexturesLength"

/*
typedef struct AllActuallConfigFileContentValues
{
	//[AppWindowConfig]
	int64_t _Width;
	int64_t _Length;
	std::string _Ratio; //maybe remove!
	int64_t _TextureScalingCoefficient;
	std::string _WindowTitle;
	int64_t _FpsCap;

	//[AppConfig]
	std::string _AppDir;
	bool _RuntimeLogEnabled;
	std::string _LogDir;
	int64_t _MaxLogFiles;

	//[AppAssetConfig]
	std::string _GraphicsDir;
	std::string _TextureFormat;
	std::string _PlayerTextures;
	int64_t _PlayerTexturesWidth; //maybe remove!
	int64_t _PlayerTexturesLength; //maybe remove!
	std::string _Level1Textures;
	int64_t _Level1TexturesWidth; //maybe remove!
	int64_t _Level1TexturesLength; //maybe remove!
}
Config;

static Config ConfigValues;
*/

#endif