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
#include "dialogs/DialogTemplate.h"
#include "dialogs/DialogTemplateTypes.h"
#include "dialogs/DialogStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class DialogTemplateReader : public DialogTemplateTypes 
	{	
		std::span<std::byte const>  Input;
		std::byte const*            Position;

	public:
		explicit
		DialogTemplateReader(std::span<std::byte const> source) 
			: Input{source}, Position{source.data()}
		{}

	public:
		DialogTemplate
		readTemplate() 
		{
			TemplateIdent constexpr
			static DLG_TEMPLATE_EX {1};

			DialogTemplate dlg {};
		
			// Ensure layout is DLG-TEMPLATE-EX
			auto* ver = this->peekTemplateIdent();
			bool const extended = *ver == DLG_TEMPLATE_EX;

			// DLG-TEMPLATE
			if (extended) {
				this->readObject<TemplateIdent>();
				dlg.HelpId = this->readObject<uint32_t>();
				dlg.ExtendedStyle = this->readObject<ExWindowStyle>();
				dlg.Style = this->readObject<WindowStyle>();
			}
			else {
				dlg.Style = this->readObject<WindowStyle>();
				dlg.ExtendedStyle = this->readObject<ExWindowStyle>();
			}
		
			dlg.NumControls = this->readObject<uint16_t>();
		
			{
				auto const rc = this->readObject<SmallRect>(); 
				dlg.Area = Rect{{rc.Left, rc.Top}, {rc.Width, rc.Height}};
			}
		
			// wchar menu[], wchar class[], wchar title[]
			dlg.Menu = this->readResourceIdent();
			dlg.ClassName = this->readResourceIdent();
			dlg.Title = this->readResourceIdent();

			// [style & DS_FONT] short ptSize, wchar font-name[]
			if (dlg.Style.test(DialogStyle::SetFont|DialogStyle::ShellFont)) {
				dlg.Height = this->readObject<int16_t>();
				if (extended) {
					dlg.Weight = this->readObject<uint16_t>();
					dlg.Italic = this->readObject<uint8_t>();
					dlg.CharSet = this->readObject<uint8_t>();
				}
				dlg.Font = this->readResourceIdent();
			}
		
			// DLG-ITEM-TEMPLATE(s)
			for (unsigned idx = 0; idx < dlg.NumControls; ++idx) 
			{
				DialogItemTemplate ctrl;

				// Each item is aligned on 32-bit boundaries
				this->Position += (reinterpret_cast<uintptr_t>(this->Position) % sizeof(uint32_t));
			
				if (extended) {
					ctrl.HelpId = this->readObject<uint32_t>();
					ctrl.ExtendedStyle = this->readObject<ExWindowStyle>();
					ctrl.Style = this->readObject<WindowStyle>();
				}
				else {
					ctrl.Style = this->readObject<WindowStyle>();
					ctrl.ExtendedStyle = this->readObject<ExWindowStyle>();
				}
			
				{
					auto rc = this->readObject<SmallRect>(); 
					ctrl.Area = Rect{{rc.Left, rc.Top}, {rc.Width, rc.Height}};
				}
			
				if (extended) {
					ctrl.Ident = this->readObject<uint32_t>();
				}
				else {
					ctrl.Ident = this->readObject<uint16_t>();
				}
			
				// Class & Text & Data fields must be aligned on 16-bit boundary
				//   https://learn.microsoft.com/en-us/windows/win32/api/Winuser/ns-winuser-dlgitemtemplate
				ctrl.ClassName = this->readResourceIdent();
				ctrl.Text = this->readResourceIdent();
				ctrl.Data = this->readBinaryData();

				// Done
				dlg.Controls.push_back(ctrl);
			}

			return dlg;
		}

	protected:
		const TemplateIdent*
		peekTemplateIdent() const
		{
			return reinterpret_cast<TemplateIdent const*>(this->Position);
		}

	protected:
		ResourceIdOrString
		readResourceIdent() 
		{
			auto* r = reinterpret_cast<VarLengthField const*>(this->Position);

			if (r->Type == ElementId::Missing) {
				this->Position += sizeof(MissingIdent);
				return ResourceIdOrString{};
			}
			else if (r->Type == ElementId::Stock) {
				this->Position += sizeof(NumericIdent);
				return ResourceIdOrString{win::ResourceId{r->Numeric.Ordinal}};
			}
			else {
				std::wstring name = r->String.Text;
				this->Position += nstd::sizeof_n<wchar_t>(name.length() + 1);
				return ResourceIdOrString{std::move(name)};
			}
		}
	
		std::vector<std::byte>
		readBinaryData() 
		{
			auto* r = reinterpret_cast<VarLengthField const*>(this->Position);

			if (r->Type == ElementId::Missing) {
				this->Position += sizeof(MissingIdent);
				return { };
			}
			else {
				std::vector<std::byte> data { r->Binary.Data, r->Binary.Data + r->Binary.Length };
				this->Position += sizeof(ElementId) + r->Binary.Length;
				return { std::move(data) };
			}
		}

		template <typename Object>
		Object
		readObject() 
		{
			Object const* obj = reinterpret_cast<Object const*>(this->Position);
			this->Position += sizeof(Object);
			return *obj;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o