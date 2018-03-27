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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Clip.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       CLIP~                                      //
    // ================================================================================ //
    
    void Clip::declare()
    {
        Factory::add<Clip>("clip", &Clip::create);
    }
    
    std::unique_ptr<Object> Clip::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Clip>(model, patcher);
    }
    
    Clip::Clip(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_minimum(0.),
    m_maximum(0.)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 0)
        {
            m_minimum = args[0].getFloat();
            
            if (args.size() > 1)
            {
                m_maximum = args[1].getFloat();
            }
        }
    }
    
    void Clip::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 0)
            {
                if (args[0].isNumber())
                {
                    send(0, {std::max(m_minimum, std::min(args[0].getFloat(), m_maximum))});
                }
                else
                {
                    warning("clip inlet 1 only takes numbers");
                }
            }
            else if (index == 1)
            {
                if (args[0].isNumber())
                {
                    m_minimum = args[0].getFloat();
                    
                    if (m_minimum > m_maximum)
                    {
                        warning("clip minimum is higher than maximum");
                    }
                }
                else
                {
                    warning("clip inlet 2 only takes numbers");
                }
            }
            else if(index == 2)
            {
                if (args[0].isNumber())
                {
                    m_maximum = args[0].getFloat();
                    
                    if (m_minimum > m_maximum)
                    {
                        warning("clip maximum is lower than minimum");
                    }
                }
                else
                {
                    warning("clip inlet 3 only takes numbers");
                }
            }
        }
    }
}}
