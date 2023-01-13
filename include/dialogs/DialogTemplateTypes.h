#pragma once
#include "formsFramework.h"

namespace core::forms
{
	struct DialogTemplateTypes {
	#pragma pack (push, 1)
		enum class ElementId : uint16_t { Missing = 0, Stock = 0xffFF };
	
		struct TemplateIdent {
			uint16_t         Version;
			const ElementId  Magic = ElementId::Stock;

			satisfies(TemplateIdent,
				constexpr IsEqualityComparable,
				NotSortable
			);
		};

		struct BinaryData {
			uint16_t   Length;
			std::byte  Data[1];
		};
	
		struct MissingIdent {
			const ElementId  Type = ElementId::Missing;
		};

		struct NumericIdent {
			const ElementId  Type = ElementId::Stock;
			uint16_t         Ordinal;
		};

		struct StringIdent {
			wchar_t    Text[2];
		};

		// var-char array: 16-bit NULL / 16-bit FfFf, 16-bit Ident / Null-terminated wide string
		struct VarLengthField {
			union {
				ElementId		Type;
				MissingIdent	Missing;
				NumericIdent    Numeric;
				StringIdent     String;
				BinaryData      Binary;
			};
		};

		struct SmallRect {
			int16_t Left, Top, Width, Height;
		};
	#pragma pack (pop)
	};
}