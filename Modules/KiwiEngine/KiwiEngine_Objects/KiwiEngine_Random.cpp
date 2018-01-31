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

#include <functional>
#include <cmath>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Random.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                     RANDOM                                       //
    // ================================================================================ //
    
    void Random::declare()
    {
        Factory::add<Random>("random", &Random::create);
    }
    
    std::unique_ptr<Object> Random::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Random>(model, patcher);
    }
    
    Random::Random(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_random_generator(),
    m_random_distribution(0, 100)
    {
        m_random_generator.seed(1);
        
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 0 && args[0].isNumber())
        {
            setRange(args[0].getInt());
        }
        
        if (args.size() > 1 && args[1].isNumber())
        {
            setSeed(args[1].getInt());
        }
    }
    
    void Random::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0)
        {
            if (args[0].isBang())
            {
                send(0, {m_random_distribution(m_random_generator)});
            }
            else
            {
                warning("random inlet 1 only understand bang");
            }
        }
        else if (index == 1)
        {
            if (args[0].isNumber())
            {
                setRange(args[0].getInt());
            }
            else
            {
                warning("random inlet 2 only understand numbers");
            }
        }
        else if (index == 2)
        {
            if (args[0].isNumber())
            {
                setSeed(args[0].getInt());
            }
            else
            {
                warning("random inlet 2 only understand numbers");
            }
        }
    }
    
    void Random::setRange(int range)
    {
        m_random_distribution.param(std::uniform_int_distribution<int>::param_type(0, std::max(0, range)));
    }
    
    void Random::setSeed(int seed)
    {
        m_random_generator.seed();
    }
}}
