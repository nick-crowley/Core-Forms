#pragma once
#include "library/core.Forms.h"
#include "nstd/Functional.h"

namespace core::forms
{
	template <typename Signature>
	using Delegate = nstd::function<Signature>;

	using NullaryDelegate = Delegate<void()>;
}