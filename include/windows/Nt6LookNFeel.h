#pragma once
#include "windows/LookNFeelProvider.h"
#include "graphics/Font.h"

namespace core::forms
{
	class FormsExport Nt6LookNFeel : public LookNFeelProvider
	{
		using base = LookNFeelProvider;

	private:
		Font  SegoeUi;

	public:
		Nt6LookNFeel();

	public:
		using base::draw;

		void
		draw(Dialog& dlg, PaintWindowEventArgs const& args) override;
	
		void
		draw(Window& wnd, PaintNonClientEventArgs const& args) override;
		
		void
		initialize(Dialog& dlg, InitDialogEventArgs const& args) override;
	};
}	// namespace core::forms