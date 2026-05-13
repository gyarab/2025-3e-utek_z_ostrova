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
	float _Vector[2] = { NULL, NULL };
	//Bool that tells whether the entity is alive [hp > 0] or not
	bool _IsAlive = false;
	//
	bool _IsRenderable = false;
	
	//Rectange defining both position and size of the entity [not its texture directly]
	SDL_FRect _Hitbox = SDL_FRect();
	//Bool that tells whether the entity is immortal or not
	bool _IsImmortal = false;
	//A non-negative number that tells how many hp does the entity have [0 = dead]
	uint64_t _Health = NULL;
	//A non-negative number that tells how many hp can the entity take away from another entity [0 = no damage]
	uint64_t _Damage = NULL;
	//A non-negative number that tells how much will the entity move on the X-axis
	float _X_Movespeed = NULL;
	//A non-negative number that tells how much will the entity move on the Y-axis
	float _Y_Movespeed = NULL;

	//All posible textures the entity can use to display itself
	TCluster _Textures = TCluster();

public:
	//Function that loads the basic info from config file on how entities should be processed
	void load_basic_info(const std::string& _SpecificCFG_Filename);
	//Function that sets the vector x direction to left by multipling X_Movespeed by -1
	void set_vector_direction_to_left(void);
	//Function that sets the vector x direction to right by multipling X_Movespeed by 1
	void set_vector_direction_to_right(void);
	//Function that sets the vector y direction to up by multipling Y_Movespeed by -1
	void set_vector_direction_to_up(void);
	//Function that sets the vector y direction to down by multipling Y_Movespeed by 1
	void set_vector_direction_to_down(void);
	//Function that sets the vector x direction to be zero - entity is not moving
	void nullify_vector_x_direction(void);
	//Function that sets the vector y direction to be zero - entity is not moving
	void nullify_vector_y_direction(void);
	//Function that tells the vector x direction my dividing it by X_Movespeed [0 = no direction ; -1 = left ; 1 = right]
	int64_t tell_vector_x_direction(void);
	//Function that tells the vector y direction my dividing it by Y_Movespeed [0 = no direction ; -1 = up ; 1 = down]
	int64_t tell_vector_y_direction(void);
	//Function that copies all textures of this entity to a different entity - should be called only when preparing new ECluster
	void copy_textures_to(entity& _AnotherEntity);
	//Function that changes _Hitbox position based on the _Vector values once only if _IsMoving is true and its alive or immortal
	void make_one_movement(void);
	//Function that takes away health of another entitys hp [if its not immortal and its alive] based on this entitys dmg - cant go to negative
	void deal_damage_to(entity& _EntityToBeDamaged);
	//Function that determines whether on entitys hitbox is inside the onther entitys hitbox or vise versa
	bool hitbox_is_touching_hitbox_of(entity& _AnotherEntity);

	//Temp!!!
	bool hitbox_is_touching_hitbox_on_left(entity& _AnotherEntity);
	//
	bool hitbox_is_touching_hitbox_on_right(entity& _AnotherEntity);
	//
	bool hitbox_is_touching_hitbox_on_up(entity& _AnotherEntity);
	//
	bool hitbox_is_touching_hitbox_on_down(entity& _AnotherEntity);
	
	//Temp!!!
	bool hitbox_is_touching_hitbox_of(hitbox& _AnotherHitbox);
	//
	bool hitbox_is_touching_hitbox_on_left(hitbox& _AnotherHitbox);
	//
	bool hitbox_is_touching_hitbox_on_right(hitbox& _AnotherHitbox);
	//
	bool hitbox_is_touching_hitbox_on_up(hitbox& _AnotherHitbox);
	//
	bool hitbox_is_touching_hitbox_on_down(hitbox& _AnotherHitbox);

	//
	void render_itself(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed);
	//

};

//Operator for function "hitbox_is_inside_another_hitbox_of"
bool operator<=>(entity& _Left, entity& _Right);

#endif