#pragma once
#include "lookNfeel/LookNFeelProvider.h"
#include "graphics/Graphics.h"

namespace core::forms
{
	class FormsExport Nt6LookNFeel : public LookNFeelProvider, 
	                                 public std::enable_shared_from_this<ILookNFeelProvider>
	{
		using base = LookNFeelProvider;
		
	public:
		std::shared_ptr<ILookNFeelProvider> const
		static Instance;

	private:
		SystemBrush  Background = SystemBrush::Dialog;
		Font  SegoeUi;

	public:
		Nt6LookNFeel();

	public:
		using base::draw;
		
		void
		draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(Dialog& dlg, PaintWindowEventArgs const& args) override;
	
		void
		draw(Window& wnd, PaintNonClientEventArgs const& args) override;
		
		void
		initialize(Dialog& dlg, InitDialogEventArgs const& args) override;
	};
}	// namespace core::forms