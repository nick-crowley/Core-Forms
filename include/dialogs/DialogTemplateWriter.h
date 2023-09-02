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
#include "dialogs/DialogTemplateBlob.h"
#include "dialogs/DialogTemplateTypes.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class DialogTemplateWriter : public DialogTemplateTypes
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		bool constexpr
		inline static AlwaysUseDlgTemplateEx = true;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		std::span<std::byte>	bufferRemaining;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(DialogTemplateWriter,
			IsDefaultConstructible,
			IsMovable,
			NotCopyable,
			NotEqualityComparable,
			NotSortable,
			NotArithmetic
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		size_t
		static measureDialogTemplate(DialogTemplate const& dlg) 
		{
			// DLG-TEMPLATE
			auto totalSize = nstd::sizeof_v<TemplateIdent,uint32_t,ExWindowStyle,WindowStyle,uint16_t,SmallRect>
			               + DialogTemplateWriter::measureResourceIdent(dlg.Menu)
			               + DialogTemplateWriter::measureResourceIdent(dlg.ClassName)
			               + DialogTemplateWriter::measureResourceIdent(dlg.Title);
			
			// [style & DS_FONT] short ptSize, wchar font-name[]
			if (dlg.Style.test(DialogStyle::SetFont|DialogStyle::ShellFont)) 
				totalSize += nstd::sizeof_v<uint16_t,uint16_t,uint8_t,uint8_t> 
				           + DialogTemplateWriter::measureResourceIdent(dlg.Font);

			// DLG-ITEM-TEMPLATE(s)
			for (const DialogItemTemplate& ctrl : dlg.Controls) {
				totalSize += totalSize % sizeof(uint32_t);
				totalSize += measureItemTemplate(ctrl);
			}

			return totalSize;
		}
		
		size_t
		static measureItemTemplate(DialogItemTemplate const& ctrl) 
		{
			return nstd::sizeof_v<uint32_t,ExWindowStyle,WindowStyle,SmallRect,uint32_t> 
			     + DialogTemplateWriter::measureResourceIdent(ctrl.ClassName)
			     + DialogTemplateWriter::measureResourceIdent(ctrl.Title)
			     + DialogTemplateWriter::measureBinaryData(ctrl.Data);
		}

		size_t
		static measureResourceIdent(std::optional<win::ResourceId> const& r) 
		{	
			if (!r) {
				return sizeof(MissingIdent);
			}
			else if (r->is_numeric()) {
				return sizeof(NumericIdent);
			}
			else 
				return nstd::sizeof_n<wchar_t>(1+r->as_string().length());
		}
		
		size_t
		static measureBinaryData(std::span<std::byte const> data) 
		{	
			if (data.empty())
				return sizeof(MissingIdent);
			else 
				return nstd::sizeof_v<uint16_t,uint16_t> + data.size();
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		DialogTemplateBlob
		writeTemplate(DialogTemplate const& dlg) 
		{
			TemplateIdent constexpr 
			static DLG_TEMPLATE_EX{ 1 };

			std::vector<std::byte> buffer(DialogTemplateWriter::measureDialogTemplate(dlg));
			this->bufferRemaining = buffer;

			// DLG-TEMPLATE
			if (DialogTemplateWriter::AlwaysUseDlgTemplateEx) {
				this->writeObject<TemplateIdent>(DLG_TEMPLATE_EX);
				this->writeObject<uint32_t>(*dlg.HelpId);
				this->writeObject<ExWindowStyle>(dlg.ExtendedStyle);
				this->writeObject<WindowStyle>(dlg.Style);
			}
			else {
				this->writeObject<WindowStyle>(dlg.Style);
				this->writeObject<ExWindowStyle>(dlg.ExtendedStyle);
			}

			this->writeObject<uint16_t>(dlg.NumControls);

			this->writeObject(SmallRect{ 
				static_cast<int16_t>(dlg.Area.Left), 
				static_cast<int16_t>(dlg.Area.Top), 
				static_cast<int16_t>(dlg.Area.width()), 
				static_cast<int16_t>(dlg.Area.height()) 
			});

			// wchar menu[], wchar class[], wchar title[]
			this->writeResourceIdent(dlg.Menu);
			this->writeResourceIdent(dlg.ClassName);
			this->writeResourceIdent(dlg.Title);
			
			// [style & DS_FONT] short ptSize, wchar font-name[]
			if (dlg.Style.test(DialogStyle::SetFont|DialogStyle::ShellFont)) {
				this->writeObject<uint16_t>(*dlg.Height);
				if (DialogTemplateWriter::AlwaysUseDlgTemplateEx) {
					this->writeObject<uint16_t>(*dlg.Weight);
					this->writeObject<uint8_t>(*dlg.Italic);
					this->writeObject<uint8_t>(*dlg.CharSet);
				}
				this->writeResourceIdent(dlg.Font);
			}

			// DLG-ITEM-TEMPLATE(s)
			for (DialogItemTemplate const& ctrl : dlg.Controls)
			{
				// Ensure each item is aligned on 32-bit boundaries
				switch (reinterpret_cast<uintptr_t>(this->bufferRemaining.data()) % sizeof(uint32_t)) {
				case 1: this->writeObject<uint8_t[3]>({0,0,0}); break;
				case 2: this->writeObject<uint8_t[2]>({0,0}); break;
				case 3: this->writeObject<uint8_t[1]>({0}); break;
				}
				
				this->writeItemTemplate(ctrl);
			}

			return {buffer.data(), this->bufferRemaining.data()};
		}

	private:
		void
		writeItemTemplate(DialogItemTemplate const& ctrl) {
			Invariant(this->bufferRemaining.size() >= measureItemTemplate(ctrl));
			
			if (DialogTemplateWriter::AlwaysUseDlgTemplateEx) {
				this->writeObject<uint32_t>(*ctrl.HelpId);
				this->writeObject<ExWindowStyle>(ctrl.ExtendedStyle);
				this->writeObject<WindowStyle>(ctrl.Style);
			}
			else {
				this->writeObject<WindowStyle>(ctrl.Style);
				this->writeObject<ExWindowStyle>(ctrl.ExtendedStyle);
			}

			this->writeObject(SmallRect{ 
				static_cast<int16_t>(ctrl.Area.Left), 
				static_cast<int16_t>(ctrl.Area.Top), 
				static_cast<int16_t>(ctrl.Area.width()), 
				static_cast<int16_t>(ctrl.Area.height()) 
			});

			if (DialogTemplateWriter::AlwaysUseDlgTemplateEx) {
				this->writeObject<uint32_t>(ctrl.Ident);
			}
			else {
				this->writeObject<uint16_t>(ctrl.Ident);
			}

			// Class & Title & Data fields must be aligned on 16-bit boundary
			//   https://learn.microsoft.com/en-us/windows/win32/api/Winuser/ns-winuser-dlgitemtemplate
			this->writeResourceIdent(ctrl.ClassName);
			this->writeResourceIdent(ctrl.Title);
			this->writeBinaryData(ctrl.Data);
		}

		template <typename Object>
		void
		writeObject(Object const& value) {
			Invariant(this->bufferRemaining.size() >= sizeof(Object));

			auto const bytes = std::as_bytes(std::span(&value,1));
			this->bufferRemaining = {
				ranges::copy(bytes, this->bufferRemaining.begin()).out,
				this->bufferRemaining.end()
			};
		}
	
		void
		writeResourceIdent(std::optional<win::ResourceId> const& r) 
		{	
			Invariant(this->bufferRemaining.size() >= measureResourceIdent(r));

			if (!r) {
				this->writeObject<MissingIdent>({});
			}
			else if (r->is_numeric()) {
				this->writeObject<NumericIdent>({ElementId::Stock, r->as_number()});
			}
			else {
				for (auto const& ch : r->as_string()) {
					this->writeObject<wchar_t>(ch);
				}
				this->writeObject<wchar_t>('\0');
			}
		}
		
		void
		writeBinaryData(std::span<std::byte const> data) 
		{	
			Invariant(this->bufferRemaining.size() >= measureBinaryData(data));

			if (data.empty())
				this->writeObject<MissingIdent>({});
			else {
				// Duplicate the creation-data array size
				this->writeObject<uint16_t>(static_cast<uint16_t>(data.size() + sizeof(uint16_t)));
				this->writeObject<uint16_t>(static_cast<uint16_t>(data.size() + sizeof(uint16_t)));		// ABI compatibility with CREATESTRUCT
				this->bufferRemaining = {
					ranges::copy(data, this->bufferRemaining.begin()).out,
					this->bufferRemaining.end()
				};
			}
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o