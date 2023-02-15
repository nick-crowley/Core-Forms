#pragma once
#include "lookNfeel/LookNFeelProvider.h"

namespace core::forms
{
	class FormsExport Win31LookNFeel : public LookNFeelProvider
	{
		using base = LookNFeelProvider;

	public:
		std::shared_ptr<ILookNFeelProvider> const
		static Instance;

	public:
		satisfies(Win31LookNFeel,
			IsDefaultConstructible
		);

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