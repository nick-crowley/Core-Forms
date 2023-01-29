#pragma once
#include "library/core.Forms.h"
#include "SizePoint.h"

namespace core::forms
{
	struct Border 
	{
		Border const
		static None;

		::LONG const Left = 0, Top = 0, Right = 0, Bottom = 0;

		constexpr
		explicit
		Border(GuiMeasurement amount) noexcept 
		  : Left{amount}, Top{amount}, Right{amount}, Bottom{amount}
		{}

		constexpr
		explicit
		Border(GuiMeasurement horz, GuiMeasurement vert) noexcept 
		  : Left{horz}, Top{vert}, Right{horz}, Bottom{vert}
		{}
		
		constexpr
		explicit
		Border(GuiMeasurement left, GuiMeasurement top, GuiMeasurement right, GuiMeasurement bottom) noexcept 
		  : Left{left}, Top{top}, Right{right}, Bottom{bottom}
		{}

		constexpr 
		explicit
		Border(Size const& amount) noexcept 
		  : Left{amount.Width}, Top{amount.Height}, Right{amount.Width}, Bottom{amount.Height}
		{}
		
		satisfies(Border,
			constexpr IsDefaultConstructible noexcept,
			constexpr IsCopyConstructible noexcept,
			constexpr IsEqualityComparable noexcept,
			constexpr NotSortable noexcept
		);
	
		Border constexpr
		operator-() const {
			return Border{-this->Left, -this->Top, -this->Right, -this->Bottom};
		}
	
		//! @brief	Return scaled version of this border
		Border constexpr
		operator*(::LONG scale) const noexcept {
			return Border{this->Left*scale, this->Top*scale, this->Right*scale, this->Bottom*scale};
		}
	};

	Border constexpr
	operator*(::LONG scale, Border const& b) noexcept {
		return b * scale;
	}
	
	Border const
	inline Border::None;

	std::string
	inline to_string(Border const& b)
	{
		if (b.Left == b.Top == b.Right == b.Bottom)
			return std::format("{{Border={}}}", b.Left);
		else if (b.Left == b.Right && b.Top == b.Bottom)
			return std::format("{{Horz={} Vert={}}}", b.Left, b.Top);
		return std::format("{{{},{},{},{}}}", b.Left, b.Top, b.Right, b.Bottom);
	}
}