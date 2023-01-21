#pragma once
#include "library/core.Forms.h"
#include "graphics/Rectangle.h"

namespace core::forms
{
	std::string
	to_string(::HRGN);

	class Region {
	private:
		using type = Region;
		using reference = Region&;

	private:
		::HRGN  Handle = nullptr;

	public:
		Region() noexcept 
		  : Handle{::CreateRectRgn(0,0,0,0)}
		{}
	
		Region(Rect const& r) noexcept
		  : Handle{::CreateRectRgn(r.Left,r.Top,r.Right,r.Bottom)}
		{}
	
		Region(::HRGN rgn)  
		  : Handle{ThrowIfNull(rgn)}
		{
			if (rgn == (::HRGN)NULLREGION)
				throw invalid_argument{"Region::ctor() 'rgn' is NULLREGION"};
		}
	
		~Region() noexcept
		{
			if (this->Handle)
				::DeleteObject(this->Handle);
		}

	public:
		Region(type const& r)  
		  : Region{}
		{
			if (::CombineRgn(this->Handle, r.Handle, nullptr, RGN_COPY) == ERROR)
				win::LastError{}.throwAlways("Cannot copy region {}", to_string(r.Handle));
		}

		reference 
		operator=(type const& r) 
		{
			type{r}.swap(*this);
			return *this;
		}
	
		reference
		operator=(Rect const& r)
		{
			if (!::SetRectRgn(this->Handle, r.Left, r.Top, r.Right, r.Bottom))
				win::LastError{}.throwAlways("Cannot overwrite region with {}", to_string(r));
			return *this;
		}

		Region(type&& r) noexcept
		  : Handle{std::exchange(r.Handle,nullptr)}
		{}

		reference 
		operator=(type&& r) noexcept {
			type{std::move(r)}.swap(*this);
			return *this;
		}

	public:
		std::unique_ptr<::RGNDATA>
		static getData(::HRGN rgn) {
			auto const size = ::GetRegionData(ThrowIfNull(rgn), 0, nullptr);
			auto data = boost::reinterpret_pointer_cast<::RGNDATA>(std::make_unique<std::byte[]>(size));
			if (!::GetRegionData(rgn, size, data.get()))
				return nullptr;
			return data;
		}

	public:
		Rect
		bounds() const noexcept {
			Rect rc;
			std::ignore = ::GetRgnBox(this->Handle, rc);
			return rc;
		}

		bool 
		contains(Point const& pt) const noexcept {
			return ::PtInRegion(this->Handle, pt.X, pt.Y) != FALSE;
		}
	
		bool 
		contains(Rect const& r) const noexcept {
			return ::RectInRegion(this->Handle, r) != FALSE;
		}

		//! Equality comparable
		bool 
		operator==(type const& r) const noexcept {
			return ::EqualRgn(this->Handle, r.Handle) != FALSE;
		}
	
		bool 
		operator!=(type const& r) const noexcept {
			return ::EqualRgn(this->Handle, r.Handle) == FALSE;
		}

		//! No ordering relation
		auto constexpr
		operator<=>(type const&) const noexcept = delete;
	
		type
		operator|(type const& r) const noexcept {
			type lhs{*this};
			lhs.combine(r);
			return lhs;
		}
	
		type
		operator&(type const& r) const noexcept {
			type lhs{*this};
			lhs.intersect(r);
			return lhs;
		}
	
		type
		operator-(type const& r) const noexcept {
			type lhs{*this};
			lhs.exclude(r);
			return lhs;
		}
	
		type
		operator+(Point const& pt) const noexcept {
			type lhs{*this};
			lhs.translate(pt);
			return lhs;
		}

		/*implicit*/ operator 
		std::remove_pointer_t<::HRGN> const*() const noexcept {
			return this->Handle;
		}

	public:
		void
		combine(type const& r) noexcept {
			if (::CombineRgn(this->Handle, this->Handle, r.Handle, RGN_OR) == ERROR)
				win::LastError{}.throwAlways("Cannot union {} with {}", to_string(*this), to_string(const_cast<Region&>(r)));
		}

		::HRGN
		detach() noexcept {
			return std::exchange(this->Handle, nullptr);
		}
	
		void
		exclude(type const& r) noexcept {
			if (::CombineRgn(this->Handle, this->Handle, r.Handle, RGN_DIFF) == ERROR)
				win::LastError{}.throwAlways("Cannot combine {} with {}", to_string(*this), to_string(const_cast<Region&>(r)));
		}
	
		void
		intersect(type const& r) noexcept {
			if (::CombineRgn(this->Handle, this->Handle, r.Handle, RGN_AND) == ERROR)
				win::LastError{}.throwAlways("Cannot intersect {} with {}", to_string(*this), to_string(const_cast<Region&>(r)));
		}
	
		void
		swap(type& r) noexcept {
			std::swap(this->Handle, r.Handle);
		}
	
		void
		translate(Point const& pt) noexcept {
			if (::OffsetRgn(this->Handle, pt.X, pt.Y) == ERROR)
				win::LastError{}.throwAlways("Cannot offset {} by {}", to_string(*this), to_string(pt));
		}

		reference
		operator|=(type const& r) noexcept {
			this->combine(r);
			return *this;
		}
	
		reference
		operator&=(type const& r) noexcept {
			this->intersect(r);
			return *this;
		}
	
		reference
		operator-=(type const& r) noexcept {
			this->exclude(r);
			return *this;
		}
	
		reference
		operator+=(Point const& pt) noexcept {
			this->translate(pt);
			return *this;
		}

		/*implicit*/ operator 
		::HRGN() noexcept {
			return this->Handle;
		}
	};

	std::string
	inline to_string(::HRGN rgn)
	{
		if (auto data = Region::getData(rgn); !data)
			return "{Invalid region}";
		else if (data->rdh.iType != RDH_RECTANGLES) 
			return std::format("{{Unknown format #{}}}", data->rdh.iType);
		else {
			Rect bounds = data->rdh.rcBound;
			return std::format("{{RDH_RECTANGLES: Count={} Bounds={}x{}}}", data->rdh.nCount, bounds.width(), bounds.height());
		}
	}
}