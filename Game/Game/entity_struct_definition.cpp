#include "escape_from_the_island.hpp"

//
INLINE void entity::load_basic_info(const std::string& _SpecificDB_Filename)
{

};

//Function that changes _Hitbox position based on the _Vector values once only if _IsMoving is true
INLINE void entity::make_one_movement(void)
{
	if (!this->_IsMoving)
		return;

	this->_Hitbox.x += _Vector[DX];
	this->_Hitbox.y += _Vector[DY];

	return;
};

//Function that takes away health of another entitys hp [if its not immortal and its alive] based on this entitys dmg - cant go to negative
INLINE void entity::deal_damage_to(entity& _EntityToBeDamaged)
{
	if (_EntityToBeDamaged._Health == -1 || !_EntityToBeDamaged._IsAlive)
		return;

	_EntityToBeDamaged._Health < this->_Damage ?
		_EntityToBeDamaged._Health = NULL :
		_EntityToBeDamaged._Health -= this->_Damage;

	if (_EntityToBeDamaged._Health == NULL)
		_EntityToBeDamaged._IsAlive = false;

	return;
};

//Function that determines whether on entitys hitbox is inside the onther entitys hitbox or vise versa
INLINE bool entity::hitbox_is_inside_hitbox_of(entity& _AnotherEntity)
{
	return
		(
			(this->_Hitbox.x >= _AnotherEntity._Hitbox.x && this->_Hitbox.x <= _AnotherEntity._Hitbox.x + _AnotherEntity._Hitbox.w)
			||
			(this->_Hitbox.y >= _AnotherEntity._Hitbox.y && this->_Hitbox.y <= _AnotherEntity._Hitbox.y + _AnotherEntity._Hitbox.h)
		)
		||
		(
			(_AnotherEntity._Hitbox.x >= this->_Hitbox.x && _AnotherEntity._Hitbox.x <= this->_Hitbox.x + this->_Hitbox.w)
			||
			(_AnotherEntity._Hitbox.y >= this->_Hitbox.y && _AnotherEntity._Hitbox.y <= this->_Hitbox.y + this->_Hitbox.h)
		);
};

//Operator for function "hitbox_is_inside_another_hitbox_of"
INLINE bool operator<=>(entity& _Left, entity& _Right)
{
	_Left.hitbox_is_inside_hitbox_of(_Right);
};