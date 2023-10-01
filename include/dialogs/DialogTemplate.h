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
#include "controls/ControlDictionary.h"
#include "controls/CommonControls.h"
#include "dialogs/DialogItemTemplate.h"
#include "dialogs/ResourceIdOrString.h"
#include "graphics/Rectangle.h"
#include "forms/Window.h"
#include "forms/ClassId.h"
#include "forms/WindowStyle.h"
#include "forms/ExWindowStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class DialogTemplate 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		std::optional<uint32_t>         HelpId;
		nstd::bitset<ExWindowStyle>     ExtendedStyle;
		nstd::bitset<WindowStyle>       Style;
		uint16_t                        NumControls;
		Rect                            Area;
		ResourceIdOrString              Menu;
		ResourceIdOrString              ClassName;
		ResourceIdOrString              Title;
		ResourceIdOrString              Font;
		std::optional<uint16_t>         Height;
		std::optional<uint16_t>         Weight;
		std::optional<uint8_t>          Italic;
		std::optional<uint8_t>          CharSet;
		std::vector<DialogItemTemplate> Controls;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(DialogTemplate,
			IsRegular,
			NotSortable
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		subclassControls(ControlDictionary const& bindings) 
		{
			for (auto& ctrl : this->Controls) {
				if (!ctrl.ClassName)
					continue;
				else if (!bindings.contains(ctrl.Ident)) {
					auto const id = ctrl.Ident == -1 ? "IDC_STATIC" : '#' + std::to_string(ctrl.Ident);
					clog << Warning{"{} control (id={}) is unmanaged (it will have reduced functionality)", core::to_string(forms::identifyControl(ctrl.ClassName.as_id())), id};
					continue;
				}
				
				CommonControl const kind = forms::identifyControl(ctrl.ClassName.as_id());
				switch (kind) {
				case CommonControl::Button:    ctrl.ClassName = win::ResourceId(L"Custom.BUTTON");    break;
				case CommonControl::Edit:      ctrl.ClassName = win::ResourceId(L"Custom.EDIT");      break;
				case CommonControl::Static:    ctrl.ClassName = win::ResourceId(L"Custom.STATIC");    break;
				case CommonControl::ListBox:   ctrl.ClassName = win::ResourceId(L"Custom.LISTBOX");   break;
				case CommonControl::ScrollBar: ctrl.ClassName = win::ResourceId(L"Custom.SCROLLBAR"); break;
				case CommonControl::ComboBox:  ctrl.ClassName = win::ResourceId(L"Custom.COMBOBOX");  break;
				case CommonControl::Link:      ctrl.ClassName = win::ResourceId(L"Custom.LINK");      break;
				default: throw invalid_argument{"Controls with window class {} not yet supported", win::to_string(ctrl.ClassName.as_id())};
				}
				
				// [GROUP-BOX] GroupBoxes must have the @c WindowStyle::ClipSiblings style to prevent them from over-painting
				//             the controls beneath. Add this style here rather than forcing all clients to do so themselves.
				if (kind == CommonControl::Button && ctrl.Style.test(ButtonStyle::GroupBox) && !ctrl.Style.test(WindowStyle::ClipSiblings))
					ctrl.Style |= WindowStyle::ClipSiblings;

				Window::CreationData param(bindings[ctrl.Ident]);
				ctrl.Data = param.asBytes();
			}
		}

		void
		subclassDialog(win::ResourceId name) {
			this->ClassName = name;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o