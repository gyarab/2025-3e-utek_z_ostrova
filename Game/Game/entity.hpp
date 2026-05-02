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
	//Two numbers [_Vector[0] = dx ; _Vector[1] = dy] defining the direction which the entity will be facing / moving into
	int64_t _Vector[2];
	//Bool that tells if the entity is in the moving state
	bool _IsMoving;
	//Rectange defining both position and size of the entity [not its texture directly]
	SDL_FRect _Hitbox;
	//A non-negative number that tells how many hp does the entity have [0 = dead ; -1 = immortal]
	int64_t _Health;
	//A non-negative number that tells how many hp can the entity take away from another entity [0 = no damage]
	int64_t _Damage;
	//Bool that tells whether the entity is alive [hp > 0] or not
	bool _IsAlive;

	//All posible textures the entity can use to display itself
	TCluster _Textures;

public:
	//
	INLINE void load_basic_info(const std::string& _SpecificDB_Filename);
	//Function that changes _Hitbox position based on the _Vector values once only if _IsMoving is true
	INLINE void make_one_movement(void);
	//Function that takes away health of another entitys hp [if its not immortal and its alive] based on this entitys dmg - cant go to negative
	INLINE void deal_damage_to(entity& _EntityToBeDamaged);
	//Function that determines whether on entitys hitbox is inside the onther entitys hitbox or vise versa
	INLINE bool hitbox_is_touching_hitbox_of(entity& _AnotherEntity);

	//
};

//Operator for function "hitbox_is_inside_another_hitbox_of"
INLINE bool operator<=>(entity& _Left, entity& _Right);

#endif