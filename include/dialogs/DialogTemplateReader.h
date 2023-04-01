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
				dlg.Height = this->readObject<uint16_t>();
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
			
				// Class & Title & Data fields must be aligned on 16-bit boundary
				//   https://learn.microsoft.com/en-us/windows/win32/api/Winuser/ns-winuser-dlgitemtemplate
				ctrl.ClassName = this->readResourceIdent();
				ctrl.Title = this->readResourceIdent();
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
		std::optional<ResourceId>
		readResourceIdent() 
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
				this->Position += nstd::sizeof_n<wchar_t>(name.length() + 1);
				return std::make_optional<ResourceId>(std::move(name));
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