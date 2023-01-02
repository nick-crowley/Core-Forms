#pragma once
#include "formsFramework.h"

enum class DelegateEquality { Equal, Unequal, Uncertain };

namespace detail {
	// Given CallableTarget, verify it can be called using arguments of type Signature

	template <typename CallableTarget, typename Signature>
	struct is_callable_invocable : std::false_type 
	{};

	template <typename CallableTarget, typename Return, typename... Parameters>
	struct is_callable_invocable<CallableTarget,Return(Parameters...)> : std::is_invocable<CallableTarget,Parameters...>
	{};
};

template <typename Signature>
class Delegate {
	template <typename>
	friend class Delegate;

private:
	using type = Delegate<Signature>;
	using FunctionWrapper = std::function<Signature>;
	
	class MethodIdentifier {
		using NullaryMethodPointer = void (MethodIdentifier::*)(void);
		using address_t = const void*;

		address_t   Object;
		std::byte   Method[sizeof(NullaryMethodPointer)];

	public:
		MethodIdentifier() : Object(nullptr), Method{}
		{}

		template <typename Instance, typename MemberPointer>
		MethodIdentifier(Instance* obj, MemberPointer&& pfx) 
		  : Object(static_cast<address_t>(obj)) 
		{
			auto* src = (const std::byte*)std::addressof(pfx);
			std::copy(src, src + sizeof(pfx), std::begin(this->Method));
		}
		
		satisfies(MethodIdentifier,
			IsCopyable,
			IsMovable,
			NotSortable
		);

	public:
		bool valid() const { 
			return this->Object != nullptr;
		};

		bool operator==(const MethodIdentifier& r) const {
			return this->Object == r.Object 
				&& std::equal(std::begin(this->Method), std::end(this->Method), std::begin(r.Method));
		}
	};

public:
	using result_t = typename FunctionWrapper::result_type;

private:
	FunctionWrapper   Target;
	MethodIdentifier  Ident;

public:
	template <typename CallableTarget, typename = std::enable_if_t<std::is_assignable_v<FunctionWrapper,CallableTarget>>> 
	//explicit 
	Delegate(CallableTarget&& t) 
	{
		static_assert(detail::is_callable_invocable<CallableTarget,Signature>::value);

		this->Target = std::forward<CallableTarget>(t);
	}
	
	template <typename Object, typename... Parameters>
	Delegate(Object* object, result_t (Object::* method)(Parameters...)) 
	  : Ident(object, method) 
	{
		static_assert(std::is_invocable_v<std::decay_t<Signature>,Parameters...>);

		this->Target = [=](Parameters... args) { return (object->*method)(std::forward<Parameters>(args)...); };
	}

	template <typename Object, typename... Parameters>
	Delegate(Object* object, result_t (Object::* method)(Parameters...) const) 
	  : Ident(object, method) 
	{
		static_assert(std::is_invocable_v<std::decay_t<Signature>,Parameters...>);

		this->Target = [=](Parameters... args) { return (object->*method)(std::forward<Parameters>(args)...); };
	}

	template <typename Object, typename... Parameters> 
	Delegate(const Object* object, result_t (Object::* method)(Parameters...) const) 
	  : Ident(object, method) 
	{
		static_assert(std::is_invocable_v<std::decay_t<Signature>,Parameters...>);

		this->Target = [=](Parameters... args) { return (object->*method)(std::forward<Parameters>(args)...); };
	}
	
	satisfies(Delegate,
		IsDefaultConstructible,
		IsCopyable,
		IsMovable,
		NotEqualityComparable,
		NotSortable
	);

	template <typename Other>
	Delegate(Delegate<Other> const&) = delete;

	template <typename Other>
	Delegate(Delegate<Other>&&) = delete;

public:
	template <typename Other>
	DelegateEquality 
	compare(const Delegate<Other>& r) const {
		if (!this->Ident.valid() || !r.Ident.valid()) {
			return DelegateEquality::Uncertain;
		}

		return this->Ident == r.Ident ? DelegateEquality::Equal : DelegateEquality::Unequal;
	}

	template <typename... Arguments>
	result_t 
	invoke(Arguments&&... args) const {
		static_assert(std::is_invocable_v<std::decay_t<Signature>,Arguments...>);

		return this->Target(std::forward<Arguments>(args)...);
	}

	template <typename... Arguments>
	result_t 
	operator()(Arguments&&... args) const {
		static_assert(std::is_invocable_v<std::decay_t<Signature>,Arguments...>);

		return this->invoke(std::forward<Arguments>(args)...);
	}
};

using NullaryDelegate = Delegate<void()>;
