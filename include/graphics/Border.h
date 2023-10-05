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
#include "SizePoint.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	struct Border 
	{
		Border const
		static None;

		::LONG const Left = 0, Top = 0, Right = 0, Bottom = 0;

		explicit constexpr
		Border(GuiMeasurement amount) noexcept 
		  : Left{amount}, Top{amount}, Right{amount}, Bottom{amount}
		{}

		explicit constexpr
		Border(GuiMeasurement horz, GuiMeasurement vert) noexcept 
		  : Left{horz}, Top{vert}, Right{horz}, Bottom{vert}
		{}
		
		explicit constexpr
		Border(GuiMeasurement left, GuiMeasurement top, GuiMeasurement right, GuiMeasurement bottom) noexcept 
		  : Left{left}, Top{top}, Right{right}, Bottom{bottom}
		{}

		explicit constexpr
		Border(Size const& amount) noexcept 
		  : Left{amount.Width}, Top{amount.Height}, Right{amount.Width}, Bottom{amount.Height}
		{}
		
		satisfies(Border,
			constexpr IsDefaultConstructible noexcept,
			constexpr IsCopyConstructible noexcept,
			constexpr IsEqualityComparable noexcept,
			NotCopyAssignable,
			NotSortable
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
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	Border constexpr
	operator*(::LONG scale, Border const& b) noexcept {
		return b * scale;
	}
	
	Border const
	inline Border::None;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	std::string
	inline to_string(Border const& b)
	{
		if (b.Left == b.Top == b.Right == b.Bottom)
			return std::to_string(b.Left);
		else if (b.Left == b.Right && b.Top == b.Bottom)
			return std::format("[H={} V={}]", b.Left, b.Top);
		return std::format("[{},{},{},{}]", b.Left, b.Top, b.Right, b.Bottom);
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o