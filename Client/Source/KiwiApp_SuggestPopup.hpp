/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED
#define KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED

#include <KiwiModel/KiwiModel_Factory.hpp>

#include <juce_gui_basics/juce_gui_basics.h>

#include "KiwiApp_Utils/KiwiApp_SuggestList.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                   SUGGEST POPUP                                  //
    // ================================================================================ //
    
    //! @brief Suggestion list Popup Component.
    class SuggestPopup : public juce::PopupMenu
    {
    public: // methods
        
        //! @brief Constructor.
        SuggestPopup(SuggestList::entries_t const& entries);
        
        //! @brief Destructor.
        ~SuggestPopup();
        
        // juce::Component
        //void paint(juce::Graphics& g) override;
        
    private: // methods
        
    private: // members
        
        SuggestList m_list;
    };
}

#endif // KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED
