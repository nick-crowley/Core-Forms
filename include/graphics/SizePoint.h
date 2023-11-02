/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "graphics/DrawFlags.h"
#include "graphics/Measurement.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Point 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using reference = Point&;

	public:
		Point const
		static Zero;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		::LONG X = 0;
		::LONG Y = 0;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Construct from (any combination of) values or system-metrics
		constexpr
		Point(Measurement x, Measurement y) noexcept
		  : X{x}, 
			Y{y}
		{}

		//! @brief	Construct from Windows POINT (ie. LONG pair)
		implicit constexpr
		Point(::POINT const& pt) noexcept
		  : X{static_cast<::LONG>(pt.x)}, 
			Y{static_cast<::LONG>(pt.y)}
		{}
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(Point,
			constexpr IsRegular noexcept,
			constexpr NotSortable noexcept
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Point constexpr
		static midPoint(Point const& a, Point const& b) noexcept {
			return Point{
				std::midpoint(a.X, b.X),
				std::midpoint(a.Y, b.Y)
			};
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Negation operator
		Point constexpr
		operator-() const noexcept {
			return Point{-this->X, -this->Y};
		}
	
		//! @brief	Translate by @p r
		Point constexpr
		operator+(Point const& r) const noexcept {
			return Point{this->X + r.X, this->Y + r.Y};
		}
	
		//! @brief	Translate by @p r
		Point constexpr
		operator-(Point const& r) const noexcept {
			return Point{this->X - r.X, this->Y - r.Y};
		}

		//! @brief	Scale by @p scale
		Point constexpr
		operator*(::LONG scale) const noexcept {
			return Point{this->X*scale, this->Y*scale};
		}
		
		Point constexpr
		inline friend operator*(::LONG scale, Point const& pt) noexcept {
			return pt * scale;
		}
	
		implicit constexpr
		operator ::POINT const*() const noexcept {
			static_assert(sizeof(Point)==sizeof(::POINT));
			return std::bit_cast<::POINT const*>(this);
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void constexpr 
		translate(Point const& r) noexcept {
			this->X += r.X;
			this->Y += r.Y;
		}
	
		//! @brief	Translate by @p r
		reference constexpr
		operator+=(Point const& r) noexcept {
			this->X += r.X;
			this->Y += r.Y;
			return *this;
		}
	
		//! @brief	Translate by @p r
		reference constexpr
		operator-=(Point const& r) noexcept {
			this->X -= r.X;
			this->Y -= r.Y;
			return *this;
		}
		
		implicit constexpr
		operator ::POINT*() noexcept {
			static_assert(sizeof(Point)==sizeof(::POINT));
			return std::bit_cast<::POINT*>(this);
		}
	};

	Point constexpr
	inline Point::Zero{0,0};


	class Size {
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = Size;
		using reference = Size&;

	public:
		Size const
		static Zero;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		::LONG Width = 0;
		::LONG Height = 0;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Construct from (any combination of) values or system-metrics
		constexpr
		Size(Measurement width, Measurement height) noexcept
		  : Width{width}, 
			Height{height}
		{}

		//! @brief	Construct from Windows SIZE (ie. LONG pair)
		implicit constexpr
		Size(::SIZE const& sz) noexcept
		  : Width{static_cast<::LONG>(sz.cx)}, 
			Height{static_cast<::LONG>(sz.cy)}
		{}
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(Size,
			constexpr IsRegular noexcept,
			constexpr NotSortable noexcept
		);
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Negation operator
		Size constexpr
		operator-() const noexcept {
			return Size{-this->Width, -this->Height};
		}
	
		//! @brief	Return sum of two sizes
		type constexpr
		operator+(type const& r) const noexcept {
			return {this->Width+r.Width, this->Height+r.Height};
		}
	
		//! @brief	Return difference of two sizes
		type constexpr
		operator-(type const& r) const noexcept {
			return {this->Width-r.Width, this->Height-r.Height};
		}
	
		//! @brief	Return scaled version of this size
		type constexpr
		operator*(float scale) const noexcept {
			return {static_cast<::LONG>(this->Width*scale), 
					static_cast<::LONG>(this->Height*scale)};
		}
	
		Size constexpr
		inline friend operator*(float scale, Size const& sz) noexcept {
			return sz * scale;
		}

		implicit constexpr
		operator ::SIZE const*() const noexcept {
			static_assert(sizeof(Size)==sizeof(::SIZE));
			return std::bit_cast<::SIZE const*>(this);
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Increase size by @p r
		reference constexpr
		operator+=(type const& r) noexcept {
			this->Width += r.Width;
			this->Height += r.Height;
			return *this;
		}
	
		//! @brief	Reduce size by @p r
		reference constexpr
		operator-=(type const& r) noexcept {
			this->Width -= r.Width;
			this->Height -= r.Height;
			return *this;
		}

		//! @brief	Multiply size by @p scale
		reference constexpr
		operator*=(float scale) noexcept {
			this->Width = static_cast<::LONG>(scale*this->Width);
			this->Height = static_cast<::LONG>(scale*this->Height);
			return *this;
		}

		implicit constexpr
		operator ::SIZE*() noexcept {
			static_assert(sizeof(Size)==sizeof(::SIZE));
			return std::bit_cast<::SIZE*>(this);
		}
	};

	Size constexpr
	inline Size::Zero{0,0};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	std::string
	inline to_string(Point const& pt)
	{
		return std::format("[X={} Y={}]", pt.X, pt.Y);
	}

	std::string
	inline to_string(Size const& sz)
	{
		return std::format("[Width={} Height={}]", sz.Width, sz.Height);
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o