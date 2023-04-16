#pragma once
#include "library/core.Forms.h"
#include "graphics/DrawFlags.h"
#include "graphics/Pen.h"

namespace core::forms
{
	class FormsExport StockPen : public Pen
	{
		using base = Pen;

	public:
		StockPen const
		static Black;
		
		StockPen const
		static White;
		
		StockPen const
		static Hollow;

	private:
		StockPen const
		static Internal;

	protected:
		implicit
		StockPen(StockObject obj)
			: base{SharedPen{reinterpret_cast<::HPEN>(::GetStockObject(win::DWord{obj})), weakref}}
		{}
		
	public:
		nstd::return_t<StockPen const&>
		static get(StockObject obj) {
			switch (obj) {
			default: ThrowInvalidArg(obj, "Unsupported StockObject");
			case StockObject::BlackPen:    return StockPen::Black;
			case StockObject::WhitePen:    return StockPen::White;
			case StockObject::HollowPen:   return StockPen::Hollow;
			case StockObject::InternalPen: return StockPen::Internal;
			}
		}
	};
}