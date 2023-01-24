#pragma once
#include "library/core.Forms.h"
#include "dialogs/DialogTemplate.h"
#include "dialogs/DialogTemplateTypes.h"
#include "dialogs/DialogStyle.h"

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
		// FIXME: DialogTemplateReader methods need changing to lower camel-case
		DialogTemplate
		read_template() 
		{
			TemplateIdent constexpr
			static DLG_TEMPLATE_EX {1};

			DialogTemplate dlg {};
		
			// Ensure layout is DLG-TEMPLATE-EX
			auto* ver = this->peek_template_ident();
			bool const extended = *ver == DLG_TEMPLATE_EX;

			// DLG-TEMPLATE
			if (extended) {
				this->read_object<TemplateIdent>();
				dlg.HelpId = this->read_object<uint32_t>();
				dlg.ExtendedStyle = this->read_object<ExWindowStyle>();
				dlg.Style = this->read_object<WindowStyle>();
			}
			else {
				dlg.Style = this->read_object<WindowStyle>();
				dlg.ExtendedStyle = this->read_object<ExWindowStyle>();
			}
		
			dlg.NumControls = this->read_object<uint16_t>();
		
			{
				auto const rc = this->read_object<SmallRect>(); 
				dlg.Area = Rect{{rc.Left, rc.Top}, {rc.Width, rc.Height}};
			}
		
			// wchar menu[], wchar class[], wchar title[]
			dlg.Menu = this->read_resource_ident();
			dlg.ClassName = this->read_resource_ident();
			dlg.Title = this->read_resource_ident();

			// [style & DS_FONT] short ptSize, wchar font-name[]
			if (dlg.Style.test(DialogStyle::SetFont|DialogStyle::ShellFont)) {
				dlg.Height = this->read_object<uint16_t>();
				if (extended) {
					dlg.Weight = this->read_object<uint16_t>();
					dlg.Italic = this->read_object<uint8_t>();
					dlg.CharSet = this->read_object<uint8_t>();
				}
				dlg.Font = this->read_resource_ident();
			}
		
			// DLG-ITEM-TEMPLATE(s)
			for (unsigned idx = 0; idx < dlg.NumControls; ++idx) 
			{
				DialogItemTemplate ctrl;

				// Each item is aligned on 32-bit boundaries
				this->Position += (reinterpret_cast<uintptr_t>(this->Position) % sizeof(uint32_t));
			
				if (extended) {
					ctrl.HelpId = this->read_object<uint32_t>();
					ctrl.ExtendedStyle = this->read_object<ExWindowStyle>();
					ctrl.Style = this->read_object<WindowStyle>();
				}
				else {
					ctrl.Style = this->read_object<WindowStyle>();
					ctrl.ExtendedStyle = this->read_object<ExWindowStyle>();
				}
			
				{
					auto rc = this->read_object<SmallRect>(); 
					ctrl.Area = Rect{{rc.Left, rc.Top}, {rc.Width, rc.Height}};
				}
			
				if (extended) {
					ctrl.Ident = this->read_object<uint32_t>();
				}
				else {
					ctrl.Ident = this->read_object<uint16_t>();
				}
			
				// Class & Title & Data fields must be aligned on 16-bit boundary
				//   https://learn.microsoft.com/en-us/windows/win32/api/Winuser/ns-winuser-dlgitemtemplate
				ctrl.ClassName = this->read_resource_ident();
				ctrl.Title = this->read_resource_ident();
				ctrl.Data = this->read_binary_data();

				// Done
				dlg.Controls.push_back(ctrl);
			}

			return dlg;
		}

	protected:
		const TemplateIdent*
		peek_template_ident() const
		{
			return reinterpret_cast<TemplateIdent const*>(this->Position);
		}

	protected:
		std::optional<ResourceId>
		read_resource_ident() 
		{
			auto* r = reinterpret_cast<VarLengthField const*>(this->Position);

			if (r->Type == ElementId::Missing) {
				this->Position += sizeof(MissingIdent);
				return std::nullopt;
			}
			else if (r->Type == ElementId::Stock) {
				this->Position += sizeof(NumericIdent);
				return std::make_optional<ResourceId>(r->Numeric.Ordinal);
			}
			else {
				std::wstring name = r->String.Text;
				this->Position += sizeof(wchar_t) * (name.length() + 1);
				return std::make_optional<ResourceId>(std::move(name));
			}
		}
	
		std::vector<std::byte>
		read_binary_data() 
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
		read_object() 
		{
			Object const* obj = reinterpret_cast<Object const*>(this->Position);
			this->Position += sizeof(Object);
			return *obj;
		}
	};
}