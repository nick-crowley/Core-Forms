#pragma once
#include "library/core.Forms.h"
#include "com/Annotations.h"
#include "com/Dispatch.h"
#include "windows/AccessibilityFlags.h"
	
namespace core::forms
{
	class FormsExport AccessibleDecorator : public com::implements<::IAccessible,::IDispatch,::IUnknown> {
		using base = ::IAccessible;
		
	private:
		com::shared_ptr<::IAccessible>  m_impl;

	public:
		AccessibleDecorator(com::shared_ptr<::IAccessible> impl) 
			: m_impl{std::move(ThrowIfEmpty(impl))}
		{}

	public:
#pragma region IAccessible methods
        ::HRESULT
        PASCAL accSelect(long flagsSelect, ::VARIANT varChild) override {
			return this->m_impl->accSelect(flagsSelect,varChild);
		}
        
        ::HRESULT
        PASCAL accLocation(com::out_t<long> pxLeft,
                           com::out_t<long> pyTop,
                           com::out_t<long> pcxWidth,
                           com::out_t<long> pcyHeight,
                           ::VARIANT varChild) override {
			return this->m_impl->accLocation(pxLeft,pyTop,pcxWidth,pcyHeight,varChild);
		}
        
        ::HRESULT
        PASCAL accNavigate(long navDir, ::VARIANT start, com::out_t<::VARIANT> endUpAt) override {
			return this->m_impl->accNavigate(navDir,start,endUpAt);
		}
        
        ::HRESULT
        PASCAL accHitTest(long xLeft, long yTop, com::out_t<::VARIANT> child) override {
			return this->m_impl->accHitTest(xLeft,yTop,child);
		}
        
        ::HRESULT
        PASCAL accDoDefaultAction(::VARIANT child) override {
			return this->m_impl->accDoDefaultAction(child);
		}
        
        ::HRESULT
        PASCAL get_accParent(com::out_t<::IDispatch*> parent) override {
			return this->m_impl->get_accParent(parent);
		}
        
        ::HRESULT
        PASCAL get_accChildCount(com::out_t<long> count) override {
			return this->m_impl->get_accChildCount(count);
		}
        
        ::HRESULT
        PASCAL get_accChild(::VARIANT child, com::out_t<::IDispatch*> ppv) override {
			return this->m_impl->get_accChild(child,ppv);
		}
        
        ::HRESULT
        PASCAL get_accName(::VARIANT child, com::out_t<::BSTR> name) override {
			return this->m_impl->get_accName(child,name);
		}
        
        ::HRESULT
        PASCAL put_accName(::VARIANT child, ::BSTR name) override {
			return this->m_impl->put_accName(child,name);
		}
        
        ::HRESULT
        PASCAL get_accValue(::VARIANT child, com::out_t<::BSTR> value) override {
			return this->m_impl->get_accValue(child,value);
		}
        
        ::HRESULT
        PASCAL put_accValue( ::VARIANT child, ::BSTR value) override {
			return this->m_impl->put_accValue(child,value);
		}

        ::HRESULT
        PASCAL get_accDescription(::VARIANT child, com::out_t<::BSTR> description) override {
			return this->m_impl->get_accDescription(child,description);
		}
        
        ::HRESULT
        PASCAL get_accRole(::VARIANT child, com::out_t<::VARIANT> role) override {
			return this->m_impl->get_accRole(child,role);
		}
        
        ::HRESULT
        PASCAL get_accState(::VARIANT child, com::out_t<::VARIANT> state) override {
			return this->m_impl->get_accState(child,state);
		}
        
        ::HRESULT
        PASCAL get_accHelp(::VARIANT child, com::out_t<::BSTR> help) override {
			return this->m_impl->get_accHelp(child,help);
		}
        
        ::HRESULT
        PASCAL get_accHelpTopic(com::out_t<::BSTR> helpFile, ::VARIANT child, com::out_t<long> idTopic) override {
			return this->m_impl->get_accHelpTopic(helpFile,child,idTopic);
		}
        
        ::HRESULT
        PASCAL get_accKeyboardShortcut(::VARIANT child, com::out_t<::BSTR> keyboardShortcut) override {
			return this->m_impl->get_accKeyboardShortcut(child,keyboardShortcut);
		}
        
        ::HRESULT
        PASCAL get_accFocus(com::out_t<::VARIANT> child) override {
			return this->m_impl->get_accFocus(child);
		}
        
        ::HRESULT
        PASCAL get_accSelection(com::out_t<::VARIANT> children) override {
			return this->m_impl->get_accSelection(children);
		}
        
        ::HRESULT
        PASCAL get_accDefaultAction(::VARIANT child, com::out_t<::BSTR> defaultAction) override {
			return this->m_impl->get_accDefaultAction(child,defaultAction);
		}
#pragma endregion
		
#pragma region IDispatch methods
        ::HRESULT 
        PASCAL GetTypeInfoCount(com::retval_t<::UINT> count) override {
			return this->m_impl->GetTypeInfoCount(count);
        }
        
        ::HRESULT 
        PASCAL GetTypeInfo(::UINT iTInfo, ::LCID lcid, com::retval_t<::ITypeInfo*> ppv) override {
			return this->m_impl->GetTypeInfo(iTInfo,lcid,ppv);
        }
        
        ::HRESULT
        PASCAL GetIDsOfNames(::GUID const& iid, com::in_t<wchar_t*> names, ::UINT count, ::LCID lcid, com::retval_t<::DISPID> ppv) override {
			return this->m_impl->GetIDsOfNames(iid,names,count,lcid,ppv);
        }
        
        ::HRESULT 
        PASCAL Invoke(::DISPID                 member, 
                      ::GUID const&            iid, 
                      ::LCID                   lcid, 
                      ::WORD                   flags, 
                      com::out_t<::DISPPARAMS> params, 
                      com::out_t<::VARIANT>    result, 
                      com::out_t<::EXCEPINFO>  except, 
                      com::out_t<::UINT>       argErr) override {
            return this->m_impl->Invoke(member, iid, lcid, flags, params, result, except, argErr);
        }
#pragma endregion
	};

#ifdef UNUSED
	class FormsExport AccessibleAdapter : public com::adapter<::IAccessible> {
		using base = com::adapter<interface_type>;

	public:
		const_property_t<::IDispatch*>                       accParent;
		const_property_t<long>                               accChildCount;
		method_t<1, ::::VARIANT, com::out_t<::IDispatch*>>     accChild;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accName;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accValue;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accDescription;
		method_t<1, ::::VARIANT, com::out_t<::::VARIANT>>        accRole;
		/*method_t<1, ::::VARIANT, com::out_t<::::VARIANT>>        accState;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accHelp;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accKeyboardShortcut;
		const_property_t<::::VARIANT>                          accFocus;
		const_property_t<::::VARIANT>                          accSelection;
		method_t<1, ::::VARIANT, com::out_t<::::BSTR>>           accDefaultAction;
		method_t<1, long, ::::VARIANT, com::out_t<::::VARIANT>>  accNavigate;
		method_t<1, long, long, com::out_t<::::VARIANT>>       accHitTest;
		method_t<0, ::::VARIANT>                               accDoDefaultAction;
		method_t<0, com::out_t<long>, com::out_t<long>, com::out_t<long>, com::out_t<long> ::::VARIANT>   accLocation;*/

	public:
		AccessibleAdapter(com::shared_ptr<interface_type> ptr) 
		  : base{ptr},
			accParent{this->property(&interface_type::get_accParent)},
			accChildCount{this->property(&interface_type::get_accChildCount)},
			accChild{this->method<1>(&interface_type::get_accChild)},
			accName{this->method<1>(&interface_type::get_accName)},
			accValue{this->method<1>(&interface_type::get_accValue)},
			accDescription{this->method<1>(&interface_type::get_accDescription)},
			accRole{this->method<1>(&interface_type::get_accRole)}
		{}
	};
#endif
}
