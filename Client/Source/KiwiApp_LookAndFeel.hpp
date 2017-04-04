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

#ifndef KIWI_APP_LOOK_AND_FEEL_HPP_INCLUDED
#define KIWI_APP_LOOK_AND_FEEL_HPP_INCLUDED

#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi
{   
    class LookAndFeel final : public juce::LookAndFeel_V3
    {
    public: // methods
        
        LookAndFeel() = default;
        
        //! @brief Returns the typeface for a certain font name and style.
        //! @details LookAndFeel doesn't cache typeface because juce has its own caching mechanism
        juce::Typeface::Ptr getTypefaceForFont(juce::Font const& font) override;
        
        ~LookAndFeel() = default;
        
    private: // deleted methods
        
        LookAndFeel(LookAndFeel const& other) = delete;
        LookAndFeel(LookAndFeel && other) = delete;
        LookAndFeel& operator=(LookAndFeel const& other) = delete;
        LookAndFeel& operator=(LookAndFeel && other) = delete;
    };
}

#endif // KIWI_APP_LOOK_AND_FEEL_HPP_INCLUDED