#pragma once
#include "library/core.Forms.h"
#include "dialogs/DialogTemplate.h"
#include "dialogs/DialogTemplateBlob.h"
#include "dialogs/DialogTemplateTypes.h"

namespace core::forms
{
	class DialogTemplateWriter : public DialogTemplateTypes
	{
		std::byte*	Position = nullptr;

	public:
		DialogTemplateWriter() = default;

	public:
		DialogTemplateBlob
		writeTemplate(DialogTemplate const& dlg) 
		{
			TemplateIdent constexpr 
			static DLG_TEMPLATE_EX{ 1 };

			std::vector<std::byte> blob(2048);	// TODO: Measure size required by template
			this->Position = blob.data();

			// TODO: Query whether layout requires DLG-TEMPLATE-EX
			bool const extended = true;

			// DLG-TEMPLATE
			if (extended) {
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
				if (extended) {
					this->writeObject<uint16_t>(*dlg.Weight);
					this->writeObject<uint8_t>(*dlg.Italic);
					this->writeObject<uint8_t>(*dlg.CharSet);
				}
				this->writeResourceIdent(dlg.Font);
			}

			// DLG-ITEM-TEMPLATE(s)
			for (const DialogItemTemplate& ctrl : dlg.Controls)
			{
				// Ensure each item is aligned on 32-bit boundaries
				switch (reinterpret_cast<uintptr_t>(this->Position) % sizeof(uint32_t)) {
				case 1: this->writeObject<uint8_t[3]>({0,0,0}); break;
				case 2: this->writeObject<uint8_t[2]>({0,0}); break;
				case 3: this->writeObject<uint8_t[1]>({0}); break;
				}

				if (extended) {
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

				if (extended) {
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

			// TODO: Measure size required by dialog template
			assert(this->Position-blob.data() < (ptrdiff_t)blob.size());

			return {blob.data(), this->Position};
		}

	private:
		template <typename Object>
		void
		writeObject(Object const& value) {
			auto* object = reinterpret_cast<std::byte const*>(&value);
			this->Position = std::copy(object, object + sizeof(Object), this->Position);
		}
	
		void
		writeResourceIdent(std::optional<ResourceId> const& r) 
		{	
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
			if (start == finish)
				this->writeObject<MissingIdent>({});
			else {
				// Duplicate the creation-data array size
				this->writeObject<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));
				this->writeObject<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));		// ABI compatibility with CREATESTRUCT
				this->Position = std::copy(start, finish, this->Position);
			}
		}
	};
}