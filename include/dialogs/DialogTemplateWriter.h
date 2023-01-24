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
		// FIXME: DialogTemplateWriter methods need changing to lower camel-case
		DialogTemplateBlob
		write_template(DialogTemplate const& dlg) 
		{
			TemplateIdent constexpr 
			static DLG_TEMPLATE_EX{ 1 };

			std::vector<std::byte> blob(2048);	// TODO: Measure size required by template
			this->Position = blob.data();

			// TODO: Query whether layout requires DLG-TEMPLATE-EX
			bool const extended = true;

			// DLG-TEMPLATE
			if (extended) {
				this->write_object<TemplateIdent>(DLG_TEMPLATE_EX);
				this->write_object<uint32_t>(*dlg.HelpId);
				this->write_object<ExWindowStyle>(dlg.ExtendedStyle);
				this->write_object<WindowStyle>(dlg.Style);
			}
			else {
				this->write_object<WindowStyle>(dlg.Style);
				this->write_object<ExWindowStyle>(dlg.ExtendedStyle);
			}

			this->write_object<uint16_t>(dlg.NumControls);

			this->write_object(SmallRect{ 
				static_cast<int16_t>(dlg.Area.Left), 
				static_cast<int16_t>(dlg.Area.Top), 
				static_cast<int16_t>(dlg.Area.width()), 
				static_cast<int16_t>(dlg.Area.height()) 
			});

			// wchar menu[], wchar class[], wchar title[]
			this->write_resource_ident(dlg.Menu);
			this->write_resource_ident(dlg.ClassName);
			this->write_resource_ident(dlg.Title);

			// [style & DS_FONT] short ptSize, wchar font-name[]
			if (dlg.Style.test(DialogStyle::SetFont|DialogStyle::ShellFont)) {
				this->write_object<uint16_t>(*dlg.Height);
				if (extended) {
					this->write_object<uint16_t>(*dlg.Weight);
					this->write_object<uint8_t>(*dlg.Italic);
					this->write_object<uint8_t>(*dlg.CharSet);
				}
				this->write_resource_ident(dlg.Font);
			}

			// DLG-ITEM-TEMPLATE(s)
			for (const DialogItemTemplate& ctrl : dlg.Controls)
			{
				// Ensure each item is aligned on 32-bit boundaries
				switch (reinterpret_cast<uintptr_t>(this->Position) % sizeof(uint32_t)) {
				case 1: this->write_object<uint8_t[3]>({0,0,0}); break;
				case 2: this->write_object<uint8_t[2]>({0,0}); break;
				case 3: this->write_object<uint8_t[1]>({0}); break;
				}

				if (extended) {
					this->write_object<uint32_t>(*ctrl.HelpId);
					this->write_object<ExWindowStyle>(ctrl.ExtendedStyle);
					this->write_object<WindowStyle>(ctrl.Style);
				}
				else {
					this->write_object<WindowStyle>(ctrl.Style);
					this->write_object<ExWindowStyle>(ctrl.ExtendedStyle);
				}

				this->write_object(SmallRect{ 
					static_cast<int16_t>(ctrl.Area.Left), 
					static_cast<int16_t>(ctrl.Area.Top), 
					static_cast<int16_t>(ctrl.Area.width()), 
					static_cast<int16_t>(ctrl.Area.height()) 
				});

				if (extended) {
					this->write_object<uint32_t>(ctrl.Ident);
				}
				else {
					this->write_object<uint16_t>(ctrl.Ident);
				}

				// Class & Title & Data fields must be aligned on 16-bit boundary
				//   https://learn.microsoft.com/en-us/windows/win32/api/Winuser/ns-winuser-dlgitemtemplate
				this->write_resource_ident(ctrl.ClassName);
				this->write_resource_ident(ctrl.Title);
				this->write_binary_data(ctrl.Data.data(), ctrl.Data.data() + ctrl.Data.size());
			}

			// TODO: Measure size required by dialog template
			assert(this->Position-blob.data() < (ptrdiff_t)blob.size());

			return {blob.data(), this->Position};
		}

	private:
		template <typename Object>
		void
		write_object(Object const& value) {
			auto* object = reinterpret_cast<std::byte const*>(&value);
			this->Position = std::copy(object, object + sizeof(Object), this->Position);
		}
	
		void
		write_resource_ident(std::optional<ResourceId> const& r) 
		{	
			if (!r) {
				this->write_object<MissingIdent>({});
			}
			else if (r->is_numeric()) {
				this->write_object<NumericIdent>({ElementId::Stock, r->as_number()});
			}
			else {
				for (auto const& ch : r->as_string()) {
					this->write_object<wchar_t>(ch);
				}
				this->write_object<wchar_t>('\0');
			}
		}
	
		void
		write_binary_data(std::byte const* start, std::byte const* finish) 
		{	
			if (start == finish)
				this->write_object<MissingIdent>({});
			else {
				// Duplicate the creation-data array size
				this->write_object<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));
				this->write_object<uint16_t>(static_cast<uint16_t>(finish - start + sizeof(uint16_t)));		// ABI compatibility with CREATESTRUCT
				this->Position = std::copy(start, finish, this->Position);
			}
		}
	};
}