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
		static Black;
		
		StockBrush const
		static DarkGrey;

		StockBrush const
		static Grey;
		
		StockBrush const
		static LightGrey;

		StockBrush const
		static White;

		StockBrush const
		static Hollow;
		
	private:
		StockBrush const
		static Internal;

	protected:
		implicit
		StockBrush(StockObject obj)
			: base{{reinterpret_cast<::HBRUSH>(::GetStockObject(win::DWord{obj})),weakref}}
		{}
	};
}