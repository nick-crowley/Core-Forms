/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "com/Annotations.h"
#include "com/Dispatch.h"
#include "forms/AccessibilityFlags.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport AccessibleDecorator : public com::implements<::IAccessible,::IDispatch,::IUnknown> {
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
		using base = ::IAccessible;
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		com::shared_ptr<::IAccessible>  Impl;
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
        explicit
		AccessibleDecorator(com::shared_ptr<::IAccessible> impl) 
			: Impl{std::move(ThrowIfEmpty(impl))}
		{}
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		satisfies(AccessibleDecorator,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
#pragma region IAccessible methods
        ::HRESULT
        COMAPI accSelect(long flagsSelect, ::VARIANT varChild) override {
			return this->Impl->accSelect(flagsSelect,varChild);
		}
        
        ::HRESULT
        COMAPI accLocation(com::out_t<long> pxLeft,
                           com::out_t<long> pyTop,
                           com::out_t<long> pcxWidth,
                           com::out_t<long> pcyHeight,
                           ::VARIANT varChild) override {
			return this->Impl->accLocation(pxLeft,pyTop,pcxWidth,pcyHeight,varChild);
		}
        
        ::HRESULT
        COMAPI accNavigate(long navDir, ::VARIANT start, com::out_t<::VARIANT> endUpAt) override {
			return this->Impl->accNavigate(navDir,start,endUpAt);
		}
        
        ::HRESULT
        COMAPI accHitTest(long xLeft, long yTop, com::out_t<::VARIANT> child) override {
			return this->Impl->accHitTest(xLeft,yTop,child);
		}
        
        ::HRESULT
        COMAPI accDoDefaultAction(::VARIANT child) override {
			return this->Impl->accDoDefaultAction(child);
		}
        
        ::HRESULT
        COMAPI get_accParent(com::out_t<::IDispatch*> parent) override {
			return this->Impl->get_accParent(parent);
		}
        
        ::HRESULT
        COMAPI get_accChildCount(com::out_t<long> count) override {
			return this->Impl->get_accChildCount(count);
		}
        
        ::HRESULT
        COMAPI get_accChild(::VARIANT child, com::out_t<::IDispatch*> ppv) override {
			return this->Impl->get_accChild(child,ppv);
		}
        
        ::HRESULT
        COMAPI get_accName(::VARIANT child, com::out_t<::BSTR> name) override {
			return this->Impl->get_accName(child,name);
		}
        
        ::HRESULT
        COMAPI put_accName(::VARIANT child, ::BSTR name) override {
			return this->Impl->put_accName(child,name);
		}
        
        ::HRESULT
        COMAPI get_accValue(::VARIANT child, com::out_t<::BSTR> value) override {
			return this->Impl->get_accValue(child,value);
		}
        
        ::HRESULT
        COMAPI put_accValue( ::VARIANT child, ::BSTR value) override {
			return this->Impl->put_accValue(child,value);
		}

        ::HRESULT
        COMAPI get_accDescription(::VARIANT child, com::out_t<::BSTR> description) override {
			return this->Impl->get_accDescription(child,description);
		}
        
        ::HRESULT
        COMAPI get_accRole(::VARIANT child, com::out_t<::VARIANT> role) override {
			return this->Impl->get_accRole(child,role);
		}
        
        ::HRESULT
        COMAPI get_accState(::VARIANT child, com::out_t<::VARIANT> state) override {
			return this->Impl->get_accState(child,state);
		}
        
        ::HRESULT
        COMAPI get_accHelp(::VARIANT child, com::out_t<::BSTR> help) override {
			return this->Impl->get_accHelp(child,help);
		}
        
        ::HRESULT
        COMAPI get_accHelpTopic(com::out_t<::BSTR> helpFile, ::VARIANT child, com::out_t<long> idTopic) override {
			return this->Impl->get_accHelpTopic(helpFile,child,idTopic);
		}
        
        ::HRESULT
        COMAPI get_accKeyboardShortcut(::VARIANT child, com::out_t<::BSTR> keyboardShortcut) override {
			return this->Impl->get_accKeyboardShortcut(child,keyboardShortcut);
		}
        
        ::HRESULT
        COMAPI get_accFocus(com::out_t<::VARIANT> child) override {
			return this->Impl->get_accFocus(child);
		}
        
        ::HRESULT
        COMAPI get_accSelection(com::out_t<::VARIANT> children) override {
			return this->Impl->get_accSelection(children);
		}
        
        ::HRESULT
        COMAPI get_accDefaultAction(::VARIANT child, com::out_t<::BSTR> defaultAction) override {
			return this->Impl->get_accDefaultAction(child,defaultAction);
		}
#pragma endregion
		
#pragma region IDispatch methods
        ::HRESULT 
        COMAPI GetTypeInfoCount(com::retval_t<::UINT> count) override {
			return this->Impl->GetTypeInfoCount(count);
        }
        
        ::HRESULT 
        COMAPI GetTypeInfo(::UINT iTInfo, ::LCID lcid, com::retval_t<::ITypeInfo*> ppv) override {
			return this->Impl->GetTypeInfo(iTInfo,lcid,ppv);
        }
        
        ::HRESULT
        COMAPI GetIDsOfNames(::GUID const& iid, com::in_t<wchar_t*> names, ::UINT count, ::LCID lcid, com::retval_t<::DISPID> ppv) override {
			return this->Impl->GetIDsOfNames(iid,names,count,lcid,ppv);
        }
        
        ::HRESULT 
        COMAPI Invoke(::DISPID                 member, 
                      ::GUID const&            iid, 
                      ::LCID                   lcid, 
                      ::WORD                   flags, 
                      com::out_t<::DISPPARAMS> params, 
                      com::out_t<::VARIANT>    result, 
                      com::out_t<::EXCEPINFO>  except, 
                      com::out_t<::UINT>       argErr) override {
            return this->Impl->Invoke(member, iid, lcid, flags, params, result, except, argErr);
        }
#pragma endregion
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o