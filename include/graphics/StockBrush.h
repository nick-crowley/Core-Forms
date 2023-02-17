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
	
	public:
		std::type_identity_t<StockBrush const&>
		static get(StockObject obj) {
			switch (obj) {
			default: ThrowInvalidArg(obj, "Unsupported StockObject");
			case StockObject::WhiteBrush:    return StockBrush::White;
			case StockObject::LtGreyBrush:   return StockBrush::LightGrey;
			case StockObject::GreyBrush:     return StockBrush::Grey;
			case StockObject::DkGreyBrush:   return StockBrush::DarkGrey;
			case StockObject::BlackBrush:    return StockBrush::Black;
			case StockObject::HollowBrush:   return StockBrush::Hollow;
			case StockObject::InternalBrush: return StockBrush::Internal;
			}
		}
	};
}