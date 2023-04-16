#pragma once
#include "library/core.Forms.h"
#include "graphics/DrawFlags.h"
#include "graphics/Font.h"

namespace core::forms
{
	class FormsExport StockFont : public Font
	{
		using base = Font;

	public:
		StockFont const
		static AnsiFixed;

		StockFont const
		static AnsiVar;

		StockFont const
		static DefaultGui;

		StockFont const
		static OemFixed;

		StockFont const
		static System;

		StockFont const
		static SystemFixed;

	protected:
		implicit
		StockFont(StockObject obj)
			: base{SharedFont{reinterpret_cast<::HFONT>(::GetStockObject(win::DWord{obj})), weakref}}
		{}

	public:
		nstd::return_t<StockFont const&>
		static get(StockObject obj) {
			switch (obj) {
			default: ThrowInvalidArg(obj, "Unsupported StockObject");
			case StockObject::AnsiFixedFont:    return StockFont::AnsiFixed;
			case StockObject::AnsiVarFont:      return StockFont::AnsiVar;
			case StockObject::DefaultGuiFont:   return StockFont::DefaultGui;
			case StockObject::OemFixedFont:     return StockFont::OemFixed;
			case StockObject::SystemFont:       return StockFont::System;
			case StockObject::SystemFixedFont:  return StockFont::SystemFixed;
			}
		}
	};
}