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

#include "KiwiApp_LookAndFeel.hpp"
#include "KiwiApp_BinaryData.hpp"

namespace bfonts = kiwi::binary_data::fonts;

namespace kiwi
{
    juce::Typeface::Ptr LookAndFeel::getTypefaceForFont(juce::Font const& font)
    {
        juce::Typeface::Ptr typeface;
        
        if (font.getTypefaceName() == juce::Font::getDefaultSansSerifFontName())
        {
            if (font.getStyleFlags() & bold)
            {
                if (font.getStyleFlags() & italic)
                {
                    typeface =
                    juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_BoldItalic_ttf,
                                                            bfonts::open_sans::OpenSans_BoldItalic_ttf_size);
                }
                else
                {
                    typeface =
                    juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_Bold_ttf,
                                                            bfonts::open_sans::OpenSans_Bold_ttf_size);
                }
            }
            else if(font.getStyleFlags() & italic)
            {
                typeface =
                juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_SemiboldItalic_ttf,
                                                        bfonts::open_sans::OpenSans_SemiboldItalic_ttf_size);
            }
            else
            {
                typeface =
                juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_Semibold_ttf,
                                                        bfonts::open_sans::OpenSans_Semibold_ttf_size);
            }
        }
        
        if (typeface == nullptr)
        {
            typeface = juce::Font::getDefaultTypefaceForFont (font);
        }
        
        return typeface;
    }
}
