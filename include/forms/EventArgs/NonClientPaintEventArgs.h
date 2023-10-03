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
#include "core/ObservableEvent.h"
#include "graphics/Graphics.h"
#include "forms/WindowStyle.h"
#include "forms/WindowInfo.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport Window;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{	
	enum class Coords {
		Screen, 
		Window, 
		Client,
	};

	class NonClientComponentBounds
	{
	public:
		Rect  Caption,		  //!< Window co-ordinates
			  Title,		  //!< Window co-ordinates
			  SysMenuBtn,	  //!< Window co-ordinates
			  MinimizeBtn,	  //!< Window co-ordinates
			  MaximizeBtn,	  //!< Window co-ordinates
			  Window;		  //!< Screen co-ordinates

	public:
		explicit
		NonClientComponentBounds(nstd::bitset<WindowStyle> style, Rect wnd, Coords results) noexcept
		  : Window{wnd}
		{
			ThrowIf(results, results == Coords::Client);

			// Caption
			this->Caption = Rect{0, 0, wnd.width(), SystemMetric::cyCaption};
			this->Caption.inflate(-Size{SystemMetric::cxSizeFrame, 0} * 2);
			this->Caption.translate(Point{0, SystemMetric::cySizeFrame} * 2);

			// Caption text
			Size const rcIcon {this->Caption.height(), this->Caption.height()};
			this->Title = this->Caption;
			this->Title.Left += rcIcon.Width + (LONG)GuiMeasurement{SystemMetric::cxSizeFrame}*2;
		
			// System Menu
			this->SysMenuBtn = Rect{this->Caption.topLeft(), rcIcon};
			--this->SysMenuBtn.Left; --this->SysMenuBtn.Top;

			// Caption buttons
			this->MaximizeBtn = Rect{this->Caption.topRight(), rcIcon, Rect::FromTopRight};
			if (!style.test(WindowStyle::MaximizeBox)) 
				this->MinimizeBtn = std::exchange(this->MaximizeBtn, Rect::Empty);
			else
				this->MinimizeBtn = this->MaximizeBtn - Point{rcIcon.Width,0};

			if (results == Coords::Screen) {
				this->Caption += wnd.topLeft();
				this->Title += wnd.topLeft();
				this->SysMenuBtn += wnd.topLeft();
				this->MaximizeBtn += wnd.topLeft();
				this->MinimizeBtn += wnd.topLeft();
			}
		}
	};


	class FormsExport NonClientPaintEventArgs {
	public:
		Region                         Area;
		Rect                           Bounds;
		std::optional<DeviceContext>   mutable Graphics;
		std::optional<Region>          InvalidArea;
		Window&                        Window;
		WindowCaptionState             CaptionState;
		WindowCaptionButtons           CaptionButtons;

	public:
		NonClientPaintEventArgs(forms::Window& window, ::WPARAM update, ::LPARAM) 
		  : Window{window}, 
			CaptionState{WindowCaptionState::Unknown}
		{
			if (::HRGN const rgn = reinterpret_cast<::HRGN>(update); rgn && rgn != Region::Null)
				this->InvalidArea = Region{rgn, adopt};
		}
		
		explicit
		NonClientPaintEventArgs(NonClientActivateEventArgs&& args) 
		  : InvalidArea{std::move(args.InvalidArea)},
			Window{args.Window},
			CaptionState{args.CaptionState}
		{}

		explicit
		NonClientPaintEventArgs(forms::Window& window, ::HRGN update) 
		  : Window{window},
		    CaptionState{WindowCaptionState::Active}
		{
			if (update && update != Region::Null)
				this->InvalidArea = Region{update,adopt};
		}
	
		~NonClientPaintEventArgs() noexcept 
		{
			if (this->InvalidArea)
				this->InvalidArea->detach();
		}

	public:
		bool 
		beginPaint(WindowCaptionButtons const& curButtonState);

		void 
		endPaint();
	};

	using NonClientPaintDelegate = Delegate<void (Window&,NonClientPaintEventArgs)>;
	using NonClientPaintEvent = ObservableEvent<NonClientPaintDelegate>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o