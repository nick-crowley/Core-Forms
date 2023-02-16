#pragma once
#include "library/core.Forms.h"
#include "graphics/DrawFlags.h"
#include "graphics/Brush.h"

namespace core::forms
{
	class FormsExport StockBrush : public Brush
	{
		using base = Brush;

	public:
		StockBrush const
		static BlackBrush;
		
		StockBrush const
		static GreyBrush;

		StockBrush const
		static WhiteBrush;
		
		StockBrush const
		static NullBrush;

	protected:
		implicit
		StockBrush(StockObject obj)
			: base{{reinterpret_cast<::HBRUSH>(::GetStockObject(win::DWord{obj})),weakref}}
		{}
	};
}