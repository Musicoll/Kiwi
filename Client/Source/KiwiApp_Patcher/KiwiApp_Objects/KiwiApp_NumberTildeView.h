/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_NumberViewBase.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER TILDE VIEW                              //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class NumberTildeView : public NumberViewBase
    {
    public: // methods
        
        // @brief The declaration method.
        static void declare();
        
        //! @brief Creation method.
        static std::unique_ptr<ObjectView> create(model::Object & object_model);
        
        //! @brief Constructor.
        NumberTildeView(model::Object & object_model);
        
        //! @brief Destructor.
        ~NumberTildeView();
        
    private: // methods
        
        void drawIcon (juce::Graphics& g) const override;
        
        //! @brief Called when the displayed number has just changed.
        void displayNumberChanged(double new_number) override final;
        
        //! @brief Called whenever one of the object's attribute has changed.
        void parameterChanged(std::string const& name, tool::Parameter const& param) override final;
        
    private: // deleted methods
        
        NumberTildeView() = delete;
        NumberTildeView(NumberTildeView const&) = delete;
        NumberTildeView(NumberTildeView && other) = delete;
        NumberTildeView& operator=(NumberTildeView const& other) = delete;
        NumberTildeView& operator=(NumberTildeView && other) = delete;
    };
}

