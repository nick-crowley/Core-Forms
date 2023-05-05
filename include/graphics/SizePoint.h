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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class GuiMeasurement
	{
		int Value;

	public:
		template <std::integral Integral>
		constexpr
		implicit
		GuiMeasurement(Integral n) noexcept
		  : Value{static_cast<int>(n)}
		{}

		implicit
		GuiMeasurement(SystemMetric metric) noexcept
		  : Value{::GetSystemMetrics(static_cast<int>(metric))}
		{}
		
		satisfies(GuiMeasurement,
			constexpr NotDefaultConstructible noexcept,
			constexpr IsCopyable noexcept,
			constexpr IsEqualityComparable noexcept,
			constexpr IsSortable noexcept
		);

	public:
		template <std::integral Integral>
		constexpr
		implicit operator
		Integral() const noexcept {
			return static_cast<Integral>(this->Value);
		}
	};

	class Point 
	{
	public:
		Point const
		static Zero;

	public:
		::LONG X = 0;
		::LONG Y = 0;

	public:
		//! @brief	Construct from (any combination of) values or system-metrics
		constexpr
		Point(GuiMeasurement x, GuiMeasurement y) noexcept
		  : X{x}, 
			Y{y}
		{}

		//! @brief	Construct from Windows POINT (ie. LONG pair)
		constexpr
		implicit
		Point(::POINT const& pt) noexcept
		  : X{static_cast<::LONG>(pt.x)}, 
			Y{static_cast<::LONG>(pt.y)}
		{}
	
	public:
		satisfies(Point,
			constexpr IsRegular noexcept,
			constexpr NotSortable noexcept
		);

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

		implicit operator 
		::POINT const*() const noexcept {
			return reinterpret_cast<::POINT const*>(this);
		}

	public:
		void constexpr 
		translate(Point const& r) noexcept {
			this->X += r.X;
			this->Y += r.Y;
		}
	
		implicit operator 
		::POINT*() noexcept {
			return reinterpret_cast<::POINT*>(this);
		}
	};

	Point const
	inline Point::Zero{0,0};

	static_assert(sizeof(Point)==sizeof(::POINT));

	class Size {
		using type = Size;
		using reference = Size&;

	public:
		Size const
		static Zero;

	public:
		::LONG Width = 0;
		::LONG Height = 0;

	public:
		//! @brief	Construct from (any combination of) values or system-metrics
		constexpr
		Size(GuiMeasurement width, GuiMeasurement height) noexcept
		  : Width{width}, 
			Height{height}
		{}

		//! @brief	Construct from Windows SIZE (ie. LONG pair)
		constexpr
		implicit
		Size(::SIZE const& sz) noexcept
		  : Width{static_cast<::LONG>(sz.cx)}, 
			Height{static_cast<::LONG>(sz.cy)}
		{}
	
	public:
		satisfies(Size,
			constexpr IsRegular noexcept,
			constexpr NotSortable noexcept
		);
	
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
	
		implicit operator 
		::SIZE const*() const noexcept {
			return reinterpret_cast<::SIZE const*>(this);
		}

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

		implicit operator 
		::SIZE*() noexcept {
			return reinterpret_cast<::SIZE*>(this);
		}
	};

	Size const
	inline Size::Zero{0,0};

	static_assert(sizeof(Size)==sizeof(::SIZE));
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	Point constexpr
	operator*(::LONG scale, Point const& pt) noexcept {
		return pt * scale;
	}
	
	Size constexpr
	operator*(float scale, Size const& sz) noexcept {
		return sz * scale;
	}

	std::string
	inline to_string(GuiMeasurement const& value)
	{
		return std::format("{{{}}}", static_cast<int>(value));
	}

	std::string
	inline to_string(Point const& pt)
	{
		return std::format("{{X={} Y={}}}", pt.X, pt.Y);
	}

	std::string
	inline to_string(Size const& sz)
	{
		return std::format("{{Width={} Height={}}}", sz.Width, sz.Height);
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o