/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_NumberTildeView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER TILDE VIEW                              //
    // ================================================================================ //
    
    void NumberTildeView::declare()
    {
        Factory::add<NumberTildeView>("number~", &NumberTildeView::create);
    }
    
    std::unique_ptr<ObjectView> NumberTildeView::create(model::Object & object_model)
    {
        return std::make_unique<NumberTildeView>(object_model);
    }
    
    NumberTildeView::NumberTildeView(model::Object & object_model) :
    NumberViewBase(object_model)
    {
        setEditable(false);
        setIconColour(findColour(ObjectView::ColourIds::Active));
        setInterceptsMouseClicks(false, false);
    }
    
    void NumberTildeView::parameterChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "value")
        {
            setDisplayNumber(param[0].getFloat());
        }
    }
    
    void NumberTildeView::displayNumberChanged(double new_number)
    {
    }
    
    NumberTildeView::~NumberTildeView()
    {
    }
}

