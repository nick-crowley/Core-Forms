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
		com::shared_ptr<::IAccessible>  m_impl;
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		AccessibleDecorator(com::shared_ptr<::IAccessible> impl) 
			: m_impl{std::move(ThrowIfEmpty(impl))}
		{}
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
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
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o