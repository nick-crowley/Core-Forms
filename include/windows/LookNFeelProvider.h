#pragma once
#include "windows/ILookNFeelProvider.h"
#include "nstd/Bitset.h"
#include "controls/ButtonStyle.h"
#include "controls/StaticStyle.h"
#include "graphics/DrawFlags.h"
#include "windows/WindowStyle.h"
#include "windows/ExWindowStyle.h"

namespace core::forms
{
	class Size;
	class Rect;
	class DeviceContext;

	class FormsExport LookNFeelProvider : public ILookNFeelProvider
	{
	public:
		satisfies(LookNFeelProvider,
			IsDefaultConstructible
		);

	public:
		void
		draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) override;
	
		void
		draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) override;

		void
		draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) override;
	};
	
	DrawTextFlags
	calculateFlags(nstd::bitset<ButtonStyle> style) noexcept;

	DrawTextFlags
	calculateFlags(nstd::bitset<StaticStyle> style) noexcept;

	EdgeFlags
	calculateFlags(nstd::bitset<ExWindowStyle> style) noexcept;

	Size
	measureEdge(nstd::bitset<ExWindowStyle> style) noexcept;

	void
	drawWindowBorder(DeviceContext& graphics, Rect const& client, nstd::bitset<WindowStyle> style, nstd::bitset<ExWindowStyle> exStyle) noexcept;

}	// namespace core::forms