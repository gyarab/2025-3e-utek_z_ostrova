#ifndef ENTITY
#define ENTITY

//
#include "escape_from_the_island.hpp"

//Names of the indexes of the Entity _Vector
enum EntityVectorIndexes
{
	DX = 0, 
	DY = 1
};

//Container defining a state of an entity
struct entity
{
	//Two numbers [_Vector[0] = dx ; _Vector[1] = dy] defining the direction which the entity will be facing / moving into usually related to movespeed and jump power
	int64_t _Vector[2];
	//Bool that tells if the entity is in the moving state
	bool _IsMoving;
	//Bool that tells whether the entity is alive [hp > 0] or not
	bool _IsAlive;
	
	//Rectange defining both position and size of the entity [not its texture directly]
	SDL_FRect _Hitbox;
	//Bool that tells whether the entity is immortal or not
	bool _IsImmortal;
	//A non-negative number that tells how many hp does the entity have [0 = dead]
	uint64_t _Health;
	//A non-negative number that tells how many hp can the entity take away from another entity [0 = no damage]
	uint64_t _Damage;
	//A non-negative number that tells how much will the entity move
	uint64_t _Movespeed;
	//A non-negative number that tells how high the entity will jump
	uint64_t _JumpPower;

	//All posible textures the entity can use to display itself
	TCluster _Textures;

public:
	//
	INLINE void load_basic_info(const std::string& _SpecificCFG_Filename);
	//Function that changes _Hitbox position based on the _Vector values once only if _IsMoving is true and its alive or immortal
	INLINE void make_one_movement(void);
	//Function that animates the entitys TCluster and changes _Hitbox position based on the _Vector values once only if _IsMoving is true and its alive or immortal
	INLINE void make_movement_while_animating(const std::chrono::milliseconds _TextureUpdateDelay, std::mutex* _OptionalThreadMutex = nullptr);
	//Function that takes away health of another entitys hp [if its not immortal and its alive] based on this entitys dmg - cant go to negative
	INLINE void deal_damage_to(entity& _EntityToBeDamaged);
	//Function that determines whether on entitys hitbox is inside the onther entitys hitbox or vise versa
	INLINE bool hitbox_is_touching_hitbox_of(entity& _AnotherEntity);

	//
};

//Operator for function "hitbox_is_inside_another_hitbox_of"
INLINE bool operator<=>(entity& _Left, entity& _Right);

#endif