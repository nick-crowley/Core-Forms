#pragma once
#include "library/core.Forms.h"
#include "dialogs/DialogTemplate.h"
#include "dialogs/DialogTemplateBlob.h"
#include "dialogs/DialogTemplateTypes.h"

namespace core::forms
{
	class DialogTemplateWriter : public DialogTemplateTypes
	{
		bool constexpr
		inline static AlwaysUseDlgTemplateEx = true;

	private:
		std::span<std::byte>	bufferRemaining;

	public:
		DialogTemplateWriter() = default;

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
			     + DialogTemplateWriter::measureBinaryData(ctrl.Data.data(), ctrl.Data.data() + ctrl.Data.size());
		}

		size_t
		static measureResourceIdent(std::optional<ResourceId> const& r) 
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
		static measureBinaryData(std::byte const* start, std::byte const* finish) 
		{	
			if (start == finish)
				return sizeof(MissingIdent);
			else 
				return nstd::sizeof_v<uint16_t,uint16_t> + nstd::sizeof_n<uint8_t>(finish-start);
		}

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
			this->writeBinaryData(ctrl.Data.data(), ctrl.Data.data() + ctrl.Data.size());
		}

		template <typename Object>
		void
		writeObject(Object const& value) {
			Invariant(this->bufferRemaining.size() >= sizeof(Object));

			auto const* const object = reinterpret_cast<std::byte const*>(&value);
			this->bufferRemaining = {
				std::copy(object, object + sizeof(Object), this->bufferRemaining.begin()),
				this->bufferRemaining.end()
			};
		}
	
		void
		writeResourceIdent(std::optional<ResourceId> const& r) 
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
		writeBinaryData(std::byte const* start, std::byte const* finish) 
		{	
			Invariant(this->bufferRemaining.size() >= measureBinaryData(start,finish));

			if (start == finish)
				this->writeObject<MissingIdent>({});
			else {
				// Duplicate the creation-data array size
				this->writeObject<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));
				this->writeObject<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));		// ABI compatibility with CREATESTRUCT
				this->bufferRemaining = {
					std::copy(start, finish, this->bufferRemaining.begin()),
					this->bufferRemaining.end()
				};
			}
		}
	};
}