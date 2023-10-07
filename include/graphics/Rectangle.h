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
#include "Border.h"
#include "SizePoint.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Rect {
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = Rect;
		using reference = Rect&;

	public:
		enum Origin { FromTopLeft, FromTopRight, FromCentre, FromBottomLeft, FromBottomRight };

		Rect const
		static Empty;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		::LONG Left = 0;
		::LONG Top = 0;
		::LONG Right = 0;
		::LONG Bottom = 0;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Construct from (any combination of) values or system-metrics
		constexpr
		Rect(GuiMeasurement left, 
			 GuiMeasurement top, 
			 GuiMeasurement right, 
			 GuiMeasurement bottom) noexcept
		  : Left{left},
			Top{top},
			Right{right},
			Bottom{bottom}
		{}
	
		//! @brief	Construct from top-left point and dimensions
		constexpr
		Rect(Point const& origin, Size const& dimensions) noexcept
		  : Left{static_cast<::LONG>(origin.X)}, 
			Top{static_cast<::LONG>(origin.Y)}, 
			Right{static_cast<::LONG>(origin.X+dimensions.Width)}, 
			Bottom{static_cast<::LONG>(origin.Y+dimensions.Height)} 
		{}
	
		//! @brief	Construct from any point and dimensions
		constexpr
		Rect(Point const& pt, Size const& sz, Origin origin) noexcept
		{
			switch (origin) {
			case Origin::FromTopLeft:     *this = {pt.X, pt.Y, pt.X+sz.Width, pt.Y+sz.Height}; break;
			case Origin::FromTopRight:    *this = {pt.X-sz.Width, pt.Y, pt.X, pt.Y+sz.Height}; break;
			case Origin::FromBottomLeft:  *this = {pt.X, pt.Y-sz.Height, pt.X+sz.Width, pt.Y}; break;
			case Origin::FromBottomRight: *this = {pt.X-sz.Width, pt.Y-sz.Height, pt.X, pt.Y}; break;
			case Origin::FromCentre:      *this = {pt.X-(sz.Width/2), pt.Y-(sz.Height/2), 
												pt.X+(sz.Width/2), pt.Y+(sz.Height/2)}; break;
			}
		}
	
		//! @brief	Construct from Windows RECT (ie. LONG 4-tuple)
		implicit constexpr
		Rect(::RECT const& rc) noexcept
		  : Left{static_cast<::LONG>(rc.left)}, 
			Top{static_cast<::LONG>(rc.top)}, 
			Right{static_cast<::LONG>(rc.right)}, 
			Bottom{static_cast<::LONG>(rc.bottom)} 
		{}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(Rect,
			constexpr IsRegular noexcept,
			constexpr NotSortable noexcept
		);

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Point constexpr
		bottomLeft() const noexcept {
			return {this->Left, this->Bottom};
		}
	
		Point constexpr
		bottomMiddle() const noexcept {
			return Point::midPoint(this->bottomLeft(), this->bottomRight());
		}
	
		Point constexpr
		bottomRight() const noexcept {
			return {this->Right, this->Bottom};
		}

		Point constexpr
		centre() const noexcept {
			return {this->Left + this->width()/2, 
					this->Top + this->height()/2};
		}
	
		bool constexpr
		contains(Point const& pt) const noexcept {
			return pt.X >= this->Left
				&& pt.Y >= this->Top
				&& pt.X < this->Right
				&& pt.Y < this->Bottom;
		}

		::LONG constexpr
		height() const noexcept {
			return this->Bottom - this->Top;
		}

		::LONG constexpr
		width() const noexcept {
			return this->Right - this->Left;
		}
	
		Size constexpr
		size() const noexcept {
			return {this->Right - this->Left, this->Bottom - this->Top};
		}
	
		Point constexpr
		topLeft() const noexcept {
			return {this->Left, this->Top};
		}
	
		Point constexpr
		topMiddle() const noexcept {
			return Point::midPoint(this->topLeft(), this->topRight());
		}
	
		Point constexpr
		topRight() const noexcept {
			return {this->Right, this->Top};
		}

		Rect constexpr
		operator-() const noexcept {
			return Rect{-this->Left, -this->Top, -this->Right, -this->Bottom};
		}

		Rect constexpr
		operator+(Point const& pt) const noexcept {
			Rect r(*this);
			r.translate(pt);
			return r;
		}
	
		Rect constexpr
		operator-(Point const& pt) const noexcept {
			Rect r(*this);
			r.translate(-pt);
			return r;
		}
	
		Rect constexpr
		operator+(Border const& amount) const noexcept {
			return {this->Left - amount.Left,  this->Top - amount.Top,
					this->Right + amount.Right, this->Bottom + amount.Bottom};
		}
	
		Rect constexpr
		operator-(Border const& amount) const noexcept {
			return {this->Left + amount.Left,  this->Top + amount.Top,
					this->Right - amount.Right, this->Bottom - amount.Bottom};
		}
	
		Rect constexpr
		operator+(Rect const& amount) const noexcept {
			return {this->Left + amount.Left,  this->Top + amount.Top,
					this->Right + amount.Right, this->Bottom + amount.Bottom};
		}
	
		Rect constexpr
		operator-(Rect const& amount) const noexcept {
			return {this->Left - amount.Left,  this->Top - amount.Top,
					this->Right - amount.Right, this->Bottom - amount.Bottom};
		}
	
		Rect constexpr
		operator+(Size const& sz) const noexcept {
			Rect r(*this);
			r.resize(sz);
			return r;
		}
	
		implicit constexpr
		operator ::RECT const*() const noexcept {
			static_assert(sizeof(Rect)==sizeof(::RECT));
			return std::bit_cast<::RECT const*>(this);
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void constexpr
		add(Rect const& amount) noexcept {
			this->Left += amount.Left;
			this->Right += amount.Right;
			this->Top += amount.Top;
			this->Bottom += amount.Bottom;
		}

		void constexpr
		inflate(::LONG amount) noexcept {
			this->Left -= amount;
			this->Right += amount;
			this->Top -= amount;
			this->Bottom += amount;
		}

		void constexpr
		inflate(Size const& s) noexcept {
			this->Left -= s.Width;
			this->Right += s.Width;
			this->Top -= s.Height;
			this->Bottom += s.Height;
		}
	
		void constexpr
		inflate(Border const& amount) noexcept {
			this->Left -= amount.Left;
			this->Right += amount.Right;
			this->Top -= amount.Top;
			this->Bottom += amount.Bottom;
		}

		void constexpr
		translate(Point const& pt) noexcept {
			this->Left += pt.X;
			this->Right += pt.X;
			this->Top += pt.Y;
			this->Bottom += pt.Y;
		}
	
		void constexpr
		resize(Size const& s) noexcept {
			this->Right += s.Width;
			this->Bottom += s.Height;
		}
	
		void constexpr
		subtract(Rect const& amount) noexcept {
			this->Left -= amount.Left;
			this->Right -= amount.Right;
			this->Top -= amount.Top;
			this->Bottom -= amount.Bottom;
		}

		reference constexpr
		operator+=(Point const& pt) noexcept {
			this->translate(pt);
			return *this;
		}
	
		reference constexpr
		operator-=(Point const& pt) noexcept {
			this->translate(-pt);
			return *this;
		}
	
		reference constexpr
		operator+=(Rect const& rc) noexcept {
			this->add(rc);
			return *this;
		}
	
		reference constexpr
		operator-=(Rect const& rc) noexcept {
			this->subtract(rc);
			return *this;
		}
	
		reference constexpr
		operator+=(Size const& sz) noexcept {
			this->resize(sz);
			return *this;
		}
	
		implicit constexpr
		operator ::RECT*() noexcept {
			static_assert(sizeof(Rect)==sizeof(::RECT));
			return std::bit_cast<::RECT*>(this);
		}
	};

	Rect const
	inline Rect::Empty{0,0,0,0};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	std::string
	inline to_string(Rect const& r)
	{
		return std::format("[Rect=[{}x{}],[{}x{}] Area={}x{}]", r.Left,r.Top, r.Right,r.Bottom, r.width(), r.height());
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o