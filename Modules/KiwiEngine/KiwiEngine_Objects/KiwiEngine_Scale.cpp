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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Scale.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                     SCALE                                        //
    // ================================================================================ //
    
    void Scale::declare()
    {
        Factory::add<Scale>("scale", &Scale::create);
    }
    
    std::unique_ptr<Object> Scale::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Scale>(model, patcher);
    }
    
    Scale::Scale(model::Object const& model, Patcher& patcher):
    Object(model, patcher)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 3)
        {
            m_output_high = args[3].getFloat();
        }
        
        if (args.size() > 2)
        {
            m_output_low = args[2].getFloat();
        }
        
        if (args.size() > 1)
        {
            m_input_high = args[1].getFloat();
        }
        
        if (args.size() > 0)
        {
            m_input_low = args[0].getFloat();
        }
    }
    
    void Scale::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0)
        {
            if (args[0].isNumber())
            {
                m_value = args[0].getFloat();
                send(0, {scaleValue()});
            }
            else if (args[0].isBang())
            {
                send(0, {scaleValue()});
            }
            else
            {
                warning("scale inlet 1 only understands numbers and bang");
            }
        }
        else if (index == 1)
        {
            if (args[0].isNumber())
            {
                m_input_low = args[0].getFloat();
            }
            else
            {
                warning("scale inlet 2 only understands numbers");
            }
        }
        else if(index == 2)
        {
            if (args[0].isNumber())
            {
                m_input_high = args[0].getFloat();
            }
            else
            {
                warning("scale inlet 3 only understands numbers");
            }
        }
        else if(index == 3)
        {
            if (args[0].isNumber())
            {
                m_output_low = args[0].getFloat();
            }
            else
            {
                warning("scale inlet 4 only understands numbers");
            }
        }
        else if(index == 4)
        {
            if (args[0].isNumber())
            {
                m_output_high = args[0].getFloat();
            }
            else
            {
                warning("scale inlet 5 only understands numbers");
            }
        }
    }
    
    float Scale::scaleValue()
    {
        return m_output_low
        + ((m_output_high - m_output_low) / (m_input_high - m_input_low)) * (m_value - m_input_low);
    }
}}
