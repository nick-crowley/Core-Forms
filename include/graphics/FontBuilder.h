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
#include "graphics/Font.h"
#include "graphics/Graphics.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FontBuilder
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		DeviceContext const*              TargetDevice = &forms::DeviceContext::ScreenDC;
		std::optional<SharedFont>         ExistingFont;
		std::optional<std::wstring_view>  Name;
		std::optional<PointSize>          Size;
		std::optional<FontWeight>         Weight;
		std::optional<bool>               Italic;
		std::optional<bool>               Underline;
		std::optional<FontQuality>        Quality;
		std::optional<FontCharSet>        CharSet;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(FontBuilder, 
			IsSemiRegular,
			NotEqualityComparable,
			NotSortable
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Font
		build() const {
			Invariant(this->ExistingFont || (this->Name && this->Size));

			::LOGFONTW  props { sizeof(::LOGFONTW) };
			if (this->ExistingFont && !::GetObjectW(**this->ExistingFont, sizeof(::LOGFONTW), &props))
				win::LastError{}.throwAlways("GetObject() failed");
			
			if (this->Size)
				props.lfHeight = this->TargetDevice->measureFont(*this->Size);
			if (this->Weight)
				props.lfWeight = static_cast<::LONG>(*this->Weight);
			if (this->Italic)
				props.lfItalic = static_cast<::BYTE>(*this->Italic);
			if (this->Underline)
				props.lfUnderline = static_cast<::BYTE>(*this->Underline);
			if (this->Quality)
				props.lfQuality = static_cast<::BYTE>(*this->Quality);
			if (this->CharSet)
				props.lfCharSet = static_cast<BYTE>(*this->CharSet);
			if (this->Name)
				wcscpy_s(props.lfFaceName, this->Name->data());
			
			if (::HFONT const font = ::CreateFontIndirectW(&props); !font)
				win::LastError{}.throwAlways("CreateFont() failed");
			else
				return Font{SharedFont{font}};
		}

		implicit
		operator Font() const {
			return this->build();
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		FontBuilder&
		forDevice(DeviceContext const& dc) {
			this->TargetDevice = &dc;
			return *this;
		}

		FontBuilder&
		fromExisting(SharedFont f) {
			this->ExistingFont = ThrowIfEmpty(f);
			return *this;
		}
		
		FontBuilder&
		fromExisting(Font const& f) {
			this->ExistingFont = f.handle();
			return *this;
		}

		FontBuilder&
		withCharSet(FontCharSet cs) {
			this->CharSet = cs;
			return *this;
		}

		FontBuilder&
		withItalic() {
			this->Italic = true;
			return *this;
		}

		FontBuilder&
		withName(std::wstring_view n) {
			this->Name = ThrowIfEmpty(n);
			return *this;
		}

		FontBuilder&
		withQuality(FontQuality q) {
			this->Quality = q;
			return *this;
		}
		
		FontBuilder&
		withSize(PointSize pt) {
			this->Size = pt;
			return *this;
		}

		FontBuilder&
		withWeight(FontWeight w) {
			this->Weight = w;
			return *this;
		}

		FontBuilder&
		withUnderline() {
			this->Underline = true;
			return *this;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o