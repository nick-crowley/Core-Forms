#pragma once

#define CopyConstructible(type)		type(const type&)
#define MoveConstructible(type)		type(type&&)

#define CopyAssignable(type)		type& operator=(const type&)
#define MoveAssignable(type)		type& operator=(type&&)

#define Copyable(type)				CopyConstructible(type) = default; CopyAssignable(type) = default
#define Movable(type)				MoveConstructible(type) = default; MoveAssignable(type) = default
#define CopyMovable(type)			Copyable(type); Movable(type)