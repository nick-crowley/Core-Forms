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
#include "forms/MessageDictionary.h"
#include "forms/Window.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms 
{
	class Control;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms 
{
	enum class Alignment { 
		Default, 
		Left = nstd::bit<0>,
		Centre = nstd::bit<1>,
		Right = nstd::bit<2>,
		Top = nstd::bit<3>,
		Bottom = nstd::bit<4>,
		VCentre = nstd::bit<5>,
		Horizontal = Left|Centre|Right,
		Vertical = Top|Bottom|VCentre,
	};

	enum class Side { 
		None, 
		Left    = nstd::bit<0>,
		Top     = nstd::bit<1>,
		Right   = nstd::bit<2>,
		Bottom  = nstd::bit<3>,
		Centre  = nstd::bit<4>,
		VCentre = nstd::bit<5>,
	};
}
namespace core::meta
{
	metadata bool Settings<bitwise_enum, forms::Alignment> = true;
	metadata bool Settings<bitwise_enum, forms::Side> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::meta 
{
	//! @brief	Any control which exposes a @c WindowClass with an @c OriginalWndProc member
	template <typename T>
	concept SubclassedControl = std::derived_from<std::remove_reference_t<T>,forms::Control> 
		                     && requires(T&& ctrl) {
		{ctrl.wndcls().OriginalWndProc} -> std::convertible_to<::WNDPROC>;
	};
}
namespace core::forms 
{
	class Control : public Window
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using base = Window;

		enum class FakeStyle : uint32_t { None, /*Placeholder for control-defined*/ };
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		uint16_t const  Ident;
		nstd::bitset<Side>  Anchors = Side::Left|Side::Top;
		nstd::bitset<FakeStyle>  CustomStyle;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implicit
		Control(uint16_t id)
		  : Ident{id}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		satisfies(Control,
			NotCopyable,
			IsMoveConstructible noexcept,
			NotMoveAssignable,
			NotEqualityComparable,
			NotSortable
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		template <typename Self>
		bool constexpr
		subclassed(this Self&&) {
			return meta::SubclassedControl<Self>;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		nstd::bitset<Side>
		anchors() const noexcept {
			return this->Anchors;
		}
		
		nstd::bitset<Alignment>
		virtual defaultAlign() const noexcept {
			return Alignment::Left|Alignment::Top;
		}

		template <nstd::Enumeration Style>
			requires std::same_as<std::underlying_type_t<Style>, std::underlying_type_t<FakeStyle>>
		nstd::bitset<Style>
		features() const noexcept {
			return static_cast<Style>(this->CustomStyle.value());
		}
	
		uint16_t
		ident() const noexcept {
			return this->Ident;
		}
		
		bool
		virtual ownerDraw() const noexcept {
			return false;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		anchors(nstd::bitset<Side> newAnchors) noexcept {
			using enum Side;
			ThrowIf(newAnchors, newAnchors.test(Centre) && newAnchors.test(Left|Right));
			ThrowIf(newAnchors, newAnchors.test(VCentre) && newAnchors.test(Top|Bottom));
			this->Anchors = newAnchors;
		}
		
		template <nstd::Enumeration Style>
			requires std::same_as<std::underlying_type_t<Style>, std::underlying_type_t<FakeStyle>>
		void
		features(nstd::bitset<Style> newStyle) noexcept {
			this->CustomStyle = static_cast<FakeStyle>(newStyle.value());
		}
	
	protected:
		using base::onPaint;

		Response 
		virtual onControlColour(ControlColourEventArgs args) override
		{
			// Apply the look-n-feel when owner-draw is disabled
			if (args.Managed == this) {
				args.Graphics.textColour(this->textColour());
				return *this->background();
			}

			return Unhandled;
		}

		template <meta::SubclassedControl Self>
		::LRESULT
		subclassedWndProc(this Self&& self, unsigned message, std::optional<::WPARAM> w = nullopt, std::optional<::LPARAM> l = nullopt) {
			return ::CallWindowProc(self.wndcls().OriginalWndProc, self.handle(), message, w.value_or(0), l.value_or(0));
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o