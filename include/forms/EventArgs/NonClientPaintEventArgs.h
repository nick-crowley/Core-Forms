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
#include "support/ObservableEvent.h"
#include "graphics/Graphics.h"
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
		NonClientComponentBounds(Rect wnd) noexcept
		  : Window{wnd}
		{
			this->Caption = Rect{0, 0, wnd.width(), SystemMetric::cyCaption};
			this->Caption.inflate(-2*Size{SystemMetric::cxSizeFrame,0});
			this->Caption.translate(2*Point{0,SystemMetric::cySizeFrame});

			// Caption text
			Size const rcIcon {this->Caption.height(), this->Caption.height()};
			this->Title = this->Caption;
			this->Title.Left += rcIcon.Width + (LONG)GuiMeasurement{SystemMetric::cxSizeFrame}*2;
		
			// Caption buttons
			this->SysMenuBtn = Rect{this->Caption.topLeft(), rcIcon};
			--this->SysMenuBtn.Left; --this->SysMenuBtn.Top;
			this->MaximizeBtn = Rect{this->Caption.topRight(),rcIcon} - Point{rcIcon.Width,0};
			this->MinimizeBtn = Rect{this->Caption.topRight(),rcIcon} - Point{2*rcIcon.Width,0};
		}
	};


	class FormsExport NonClientPaintEventArgs {
	public:
		Region                                Area;
		Rect                                  Bounds;
		mutable std::optional<DeviceContext>  Graphics;
		std::optional<Region>                 InvalidArea;
		Window*                               Window;
		WindowCaptionState                    State;

	public:
		NonClientPaintEventArgs(forms::Window* window, ::WPARAM w, ::LPARAM) 
		  : Window{window}, 
			State{WindowCaptionState::Unknown}
		{
			if (w > NULLREGION)
				this->InvalidArea = reinterpret_cast<::HRGN>(w);
		}
	
		NonClientPaintEventArgs(NonClientActivateEventArgs const& args) 
		  : InvalidArea{args.InvalidArea},
			Window{args.Window},
			State{args.State}
		{}
	
		~NonClientPaintEventArgs() noexcept 
		{
			if (this->InvalidArea)
				this->InvalidArea->detach();
		}

	public:
		bool 
		beginPaint();

		void 
		endPaint();
	};

	using NonClientPaintDelegate = Delegate<void (Window&,NonClientPaintEventArgs)>;
	using NonClientPaintEvent = ObservableEvent<NonClientPaintDelegate>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o