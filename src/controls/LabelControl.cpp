#include "controls/LabelControl.h"
#include "dialogs/Dialog.h"

void
core::forms::LabelControl::this_Created([[maybe_unused]] Window& sender, CreateWindowEventArgs args) 
{
	// Ensure we're an owner-drawn label with a `forms::Dialog` parent
	nstd::bitset<WindowStyle> const newStyle = static_cast<WindowStyle>(args.Data->style);
	if (!(newStyle & StaticStyle::TypeMask).test(StaticStyle::OwnerDraw))
		return;
	auto* const dlg = dynamic_cast<Dialog*>(this->parent());
	if (!dlg)
		return;

	// Obtain the original style (prior to it being assigned 'OwnerDraw') from dialog template resource 
	//  and extract the alignment settings specified in the designer.
	auto const origStyle = dlg->original().Controls[this->ident()].Style;
	nstd::bitset<Alignment> alignment = origStyle.test(StaticStyle::Right)  ? Alignment::Right
		                              : origStyle.test(StaticStyle::Centre) ? Alignment::Centre
		                              :                                       Alignment::Left;
	if (origStyle.test(StaticStyle::CentreImage))
		alignment |= Alignment::VCentre;

	this->align(alignment);
}