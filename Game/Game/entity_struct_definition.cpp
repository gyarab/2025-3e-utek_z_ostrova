#include "escape_from_the_island.hpp"

//Function that loads the basic info from config file on how entities should be processed
static INLINE void LoadBasicEntitiesProps(std::string& _AssetConfigDir, int64_t& _ScalingCoefficient)
{
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tAssetConfigDir, _AssetConfigDir);
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_ScalingCoefficient, _ScalingCoefficient);

	return;
};

//Takes string that should contain exactly 4 numbers separated with commas, extracts them and forms a hitbox from them + scales it with the scaling coefficient
static SDL_FRect MakeHitboxFromCommaSeparatedString(const std::string& _BaseString, const int64_t _ScalingCoefficient)
{
	SDL_FRect ResultHitBox = SDL_FRect();
	std::stringstream BaseStringStream(_BaseString);
	std::array<float, 4> ResultNumbers = {}; //Exactly 4 numbers
	std::string OneNumber;

	for (uint64_t c = NULL; c < ResultNumbers.size(); c++)
	{
		std::getline(BaseStringStream, OneNumber, ',');
		ResultNumbers[c] = (float)_ScalingCoefficient * std::stof(OneNumber);
	}

	//Assing these number to correct places in the hitbox
	ResultHitBox.x = ResultNumbers[0];
	ResultHitBox.y = ResultNumbers[1];
	ResultHitBox.w = ResultNumbers[2];
	ResultHitBox.h = ResultNumbers[3];

	return ResultHitBox;
};

//-----<entity>-----vv

//Function that loads a specific config file and puts basic values for the entity - should be called right after creating new entity
void entity::load_basic_info(const std::string& _SpecificCFG_Filename)
{
	std::string AssetConfigDir, OneLine;
	int64_t ScalingCoefficient = NULL;
	std::array<uint64_t, 3> ResultNumbers = {}; //Exactly 3 numbers
	std::array<float, 2> ResultFloats = {}; //Exactly 2 floats

	LoadBasicEntitiesProps(AssetConfigDir, ScalingCoefficient);

	const std::string FullAssetConfigDir = SDL_GetBasePath() + AssetConfigDir + "\\";
	std::fstream EntityCFG(FullAssetConfigDir + _SpecificCFG_Filename, std::ios::in);

	std::getline(EntityCFG, OneLine);
	this->_Hitbox = MakeHitboxFromCommaSeparatedString(OneLine, ScalingCoefficient);

	for (uint64_t c = NULL; c < ResultNumbers.size(); c++)
	{
		std::getline(EntityCFG, OneLine);
		ResultNumbers[c] = std::stoull(OneLine);
	}

	for (uint64_t c = NULL; c < ResultFloats.size(); c++)
	{
		std::getline(EntityCFG, OneLine);
		ResultFloats[c] = std::stof(OneLine);
	}

	this->_IsImmortal = (ResultNumbers[0] == 1);
	this->_Health = ResultNumbers[1];
	this->_Damage = ResultNumbers[2];
	this->_X_Movespeed = ScalingCoefficient * ResultFloats[0];
	this->_Y_Movespeed = ScalingCoefficient * ResultFloats[1];

	this->_Vector[DX] = NULL;
	this->_Vector[DY] = NULL;
	this->_IsAlive = (this->_Health > 0);

	this->_Textures = TCluster(); //

	return;
};

//Function that sets the vector x direction to left by multipling X_Movespeed by -1
void entity::set_vector_direction_to_left(void)
{
	this->_Vector[DX] = -1 * this->_X_Movespeed;

	return;
};

//Function that sets the vector x direction to right by multipling X_Movespeed by 1
void entity::set_vector_direction_to_right(void)
{
	this->_Vector[DX] = 1 * this->_X_Movespeed;
	
	return;
};

//Function that sets the vector y direction to up by multipling Y_Movespeed by -1
void entity::set_vector_direction_to_up(void)
{
	this->_Vector[DY] = -1 * this->_Y_Movespeed;

	return;
};

//Function that sets the vector y direction to down by multipling Y_Movespeed by 1
void entity::set_vector_direction_to_down(void)
{
	this->_Vector[DY] = 1 * this->_Y_Movespeed;

	return;
};

//Function that sets the vector x direction to be zero - entity is not moving
void entity::nullify_vector_x_direction(void)
{
	this->_Vector[DX] = 0;

	return;
};

//Function that sets the vector y direction to be zero - entity is not moving
void entity::nullify_vector_y_direction(void)
{
	this->_Vector[DY] = 0;

	return;
};

//Function that tells the vector x direction my dividing it by X_Movespeed [0 = no direction ; -1 = left ; 1 = right]
int64_t entity::tell_vector_x_direction(void)
{
	return (int64_t)(this->_Vector[DX] / this->_X_Movespeed);
};

//Function that tells the vector y direction my dividing it by Y_Movespeed [0 = no direction ; -1 = up ; 1 = down]
int64_t entity::tell_vector_y_direction(void)
{
	return (int64_t)(this->_Vector[DY] / this->_Y_Movespeed);
};

//Function that copies all textures of this entity to a different entity - should be called only when preparing new ECluster
void entity::copy_textures_to(entity& _AnotherEntity)
{
	//Copy using copy operator=
	_AnotherEntity._Textures = this->_Textures;

	return;
};

//Function that changes _Hitbox position based on the _Vector values once only if _IsMoving is true and its alive or immortal
void entity::make_one_movement(void)
{
	if (!this->_IsAlive && !this->_IsImmortal)
		return;

	this->_Hitbox.x += _Vector[DX];
	this->_Hitbox.y += _Vector[DY];

	return;
};

//Function that takes away health of another entitys hp [if its not immortal and its alive] based on this entitys dmg - cant go to negative
void entity::deal_damage_to(entity& _EntityToBeDamaged)
{
	if (!_EntityToBeDamaged._IsImmortal || !_EntityToBeDamaged._IsAlive)
		return;

	_EntityToBeDamaged._Health < this->_Damage ?
		_EntityToBeDamaged._Health = NULL :
		_EntityToBeDamaged._Health -= this->_Damage;

	if (_EntityToBeDamaged._Health == NULL)
		_EntityToBeDamaged._IsAlive = false;

	return;
};

//Redo!
//Function that determines whether on entitys hitbox is inside the onther entitys hitbox or vise versa
bool entity::hitbox_is_touching_hitbox_of(entity& _AnotherEntity)
{
	return
		(
			(this->_Hitbox.x >= _AnotherEntity._Hitbox.x && this->_Hitbox.x <= _AnotherEntity._Hitbox.x + _AnotherEntity._Hitbox.w)
			&&
			(this->_Hitbox.y >= _AnotherEntity._Hitbox.y && this->_Hitbox.y <= _AnotherEntity._Hitbox.y + _AnotherEntity._Hitbox.h)
		)
		||
		(
			(_AnotherEntity._Hitbox.x >= this->_Hitbox.x && _AnotherEntity._Hitbox.x <= this->_Hitbox.x + this->_Hitbox.w)
			&&
			(_AnotherEntity._Hitbox.y >= this->_Hitbox.y && _AnotherEntity._Hitbox.y <= this->_Hitbox.y + this->_Hitbox.h)
		);
};

//
void entity::render_itself(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed)
{
	if (!this->_IsRenderable || this->_Textures._ActiveTexture == nullptr)
		return; //

	SDL_RenderTexture(_TextureRenderer, *this->_Textures._ActiveTexture, NULL, &this->_Hitbox);

	//Temp -> Hitbox highlight!
	if (!_F1_KeyPressed)
		return;

	//Hitbox rectangle
	SDL_SetRenderDrawColor(_TextureRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderRect(_TextureRenderer, &this->_Hitbox);
	//Hitbox diagonals
	SDL_SetRenderDrawColor(_TextureRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderLine(_TextureRenderer, this->_Hitbox.x, this->_Hitbox.y, this->_Hitbox.x + this->_Hitbox.w, this->_Hitbox.y + this->_Hitbox.h);
	SDL_RenderLine(_TextureRenderer, this->_Hitbox.x + this->_Hitbox.w, this->_Hitbox.y, this->_Hitbox.x, this->_Hitbox.y + this->_Hitbox.h);

	//Hitbox vector
	SDL_SetRenderDrawColor(_TextureRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderLine(_TextureRenderer, this->_Hitbox.x + (this->_Hitbox.w / 2), this->_Hitbox.y + (this->_Hitbox.h / 2), this->_Hitbox.x + (this->_Hitbox.w / 2) + 4 * (this->_Vector[DX]), this->_Hitbox.y + (this->_Hitbox.h / 2) + 4 * (this->_Vector[DY]));
	//
	SDL_SetRenderDrawColor(_TextureRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	return;
}

//Operator for function "hitbox_is_inside_another_hitbox_of"
bool operator<=>(entity& _Left, entity& _Right)
{
	return _Left.hitbox_is_touching_hitbox_of(_Right);
};

//-----<entity>-----^^

//-----<ECluster>-----vv

//
entity& ECluster::operator[](const uint64_t _IndexOfEntity)
{
	if (_IndexOfEntity < 0 || _IndexOfEntity >= this->_ClusterOfEntities.size())
		std::exit(-1); //Error

	return this->_ClusterOfEntities[_IndexOfEntity];
};

//
const entity& ECluster::operator[](const uint64_t _IndexOfEntity) const
{
	if (_IndexOfEntity < 0 || _IndexOfEntity >= this->_ClusterOfEntities.size())
		std::exit(-1); //Error

	return this->_ClusterOfEntities[_IndexOfEntity];
};

//-----<ECluster>-----^^