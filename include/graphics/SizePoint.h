#pragma once
#include "formsFramework.h"
#include "graphics/DrawFlags.h"

namespace core::forms
{
	class GuiMeasurement
	{
		int Value;

	public:
		constexpr
		GuiMeasurement() noexcept = delete;

		template <std::integral Integral>
		constexpr
		/*implicit*/
		GuiMeasurement(Integral n) noexcept
		  : Value{static_cast<int>(n)}
		{}

		/*implicit*/
		GuiMeasurement(SystemMetric metric) noexcept
		  : Value{::GetSystemMetrics(static_cast<int>(metric))}
		{}

		template <std::integral Integral>
		constexpr
		/*implicit*/ operator
		Integral() const noexcept {
			return static_cast<Integral>(this->Value);
		}
	};

	std::string
	inline to_string(GuiMeasurement const& value)
	{
		return std::format("{{{}}}", static_cast<int>(value));
	}

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
		/*implicit*/
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

		/*constexpr*/ 
		operator 
		::POINT const*() const noexcept {
			return reinterpret_cast<POINT const*>(this);
		}

	public:
		void constexpr 
		translate(Point const& r) noexcept {
			this->X += r.X;
			this->Y += r.Y;
		}
	
		/*implicit*/ operator 
		::POINT*() noexcept {
			return reinterpret_cast<POINT*>(this);
		}
	};

	Point constexpr
	operator*(::LONG scale, Point const& pt) noexcept {
		return pt * scale;
	}

	Point const
	inline Point::Zero{0,0};

	static_assert(sizeof(Point)==sizeof(::POINT));

	std::string
	inline to_string(Point const& pt)
	{
		return std::format("{{X={} Y={}}}", pt.X, pt.Y);
	}

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
		/*implicit*/
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
	
		/*implicit*/ operator 
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

		/*implicit*/ operator 
		::SIZE*() noexcept {
			return reinterpret_cast<::SIZE*>(this);
		}
	};

	Size const
	inline Size::Zero{0,0};

	Size constexpr
	operator*(float scale, Size const& sz) noexcept {
		return sz * scale;
	}

	static_assert(sizeof(Size)==sizeof(::SIZE));

	std::string
	inline to_string(Size const& sz)
	{
		return std::format("{{Width={} Height={}}}", sz.Width, sz.Height);
	}
}