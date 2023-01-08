#pragma once
#include "formsFramework.h"
#include "SizePoint.h"

struct Border 
{
	::LONG const Horizontal, Vertical;

	constexpr
	explicit
	Border(GuiMeasurement amount) noexcept 
	  : Horizontal{amount}, Vertical{amount}
	{}

	constexpr
	explicit
	Border(GuiMeasurement horz, GuiMeasurement vert) noexcept 
	  : Horizontal{horz}, Vertical{vert}
	{}

	constexpr 
	explicit
	Border(Size const& amount) noexcept 
	  : Horizontal{amount.Width}, Vertical{amount.Height}
	{}

	Border constexpr
	operator-() const {
		return Border{-this->Horizontal, -this->Vertical};
	}
	
	//! @brief	Return scaled version of this border
	Border constexpr
	operator*(::LONG scale) const noexcept {
		return Border{this->Horizontal*scale, this->Vertical*scale};
	}
};

Border constexpr
operator*(::LONG scale, Border const& b) noexcept {
	return b * scale;
}

std::string
inline to_string(Border const& b)
{
	return std::format("{{Horz={} Vert={}}}", b.Horizontal, b.Vertical);
}

class Rect {
public:
	Rect const
	static Empty;

	enum Origin { FromTopLeft, FromTopRight, FromCentre };

private:
	using type = Rect;
	using reference = Rect&;

public:
	::LONG Left = 0;
	::LONG Top = 0;
	::LONG Right = 0;
	::LONG Bottom = 0;

public:
	constexpr
	Rect() noexcept = default;
	
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
		case Origin::FromTopLeft:  *this = {pt.X, pt.Y, pt.X+sz.Width, pt.Y+sz.Height}; break;
		case Origin::FromTopRight: *this = {pt.X-sz.Width, pt.Y, pt.X, pt.Y+sz.Height}; break;
		case Origin::FromCentre:   *this = {pt.X-(sz.Width/2), pt.Y-(sz.Height/2), 
		                                    pt.X+(sz.Width/2), pt.Y+(sz.Height/2)}; break;
		}
	}
	
	//! @brief	Construct from Windows RECT (ie. LONG 4-tuple)
	constexpr
	/*implicit*/
	Rect(::RECT const& rc) noexcept
	  : Left{static_cast<::LONG>(rc.left)}, 
	    Top{static_cast<::LONG>(rc.top)}, 
	    Right{static_cast<::LONG>(rc.right)}, 
	    Bottom{static_cast<::LONG>(rc.bottom)} 
	{}
	
public:
	// FIXME: Use satisfies() to cleanup these constructors
	constexpr Rect(Rect const&) noexcept = default;
	constexpr Rect& operator=(Rect const&) noexcept = default;
	
public:
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
		return {this->Left - amount.Horizontal,  this->Top - amount.Vertical,
		        this->Right + amount.Horizontal, this->Bottom + amount.Vertical};
	}
	
	Rect constexpr
	operator-(Border const& amount) const noexcept {
		return {this->Left + amount.Horizontal,  this->Top + amount.Vertical,
		        this->Right - amount.Horizontal, this->Bottom - amount.Vertical};
	}
	
	Rect constexpr
	operator+(Size const& sz) const noexcept {
		Rect r(*this);
		r.resize(sz);
		return r;
	}
	
	//! Equality comparable
	bool constexpr
	operator==(Rect const&) const noexcept = default;

	//! No ordering relation
	auto constexpr
	operator<=>(Rect const&) const noexcept = delete;

	/*implicit*/ operator 
	::RECT const*() const noexcept {
		return reinterpret_cast<::RECT const*>(this);
	}

public:
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
		this->Left -= amount.Horizontal;
		this->Right += amount.Horizontal;
		this->Top -= amount.Vertical;
		this->Bottom += amount.Vertical;
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
	operator+=(Size const& sz) noexcept {
		this->resize(sz);
		return *this;
	}
	
	/*implicit*/ operator 
	::RECT*() noexcept {
		return reinterpret_cast<::RECT*>(this);
	}
};

Rect const
inline Rect::Empty{0,0,0,0};

static_assert(sizeof(Rect)==sizeof(::RECT));

std::string
inline to_string(Rect const& r)
{
	return std::format("{{Rect={{{}x{}}},{{{}x{}}} Area={}x{}}}", r.Left,r.Top, r.Right,r.Bottom, r.width(), r.height());
}
