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
    
    Random::Random(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    {
        auto const& args = model.getArguments();
        
        setRange((args.size() > 0 && args[0].isNumber()) ? args[0].getInt() : 0ll);
        setSeed((args.size() > 1 && args[1].isNumber()) ? args[1].getInt() : 0ll);
    }
    
    void Random::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(args.empty())
            return; // abort
        
        if (index == 0)
        {
            if (args[0].isBang())
            {
                send(0, {getNextRandomValue()});
            }
            else if (args[0].getString() == "seed")
            {
                if(args.size() > 1 && args[1].isNumber())
                {
                    setSeed(args[1].getInt());
                }
                else
                {
                    warning("random: seed message must be followed by an integer");
                }
            }
            else
            {
                warning("random: inlet 1 only understands bang or seed message");
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
                warning("random: inlet 2 only understands numbers");
            }
        }
    }
    
    void Random::setRange(int64_t range)
    {
        m_random_distribution.param(rnd_distribution_t::param_type(0, std::max<int64_t>(0, range - 1)));
    }
    
    void Random::setSeed(int64_t seed)
    {
        if(seed == 0)
        {
            // obtain a seed from the timer
            seed = (clock_t::now() - m_start_time).count();
        }
        
        m_random_generator.seed(seed);
    }
    
    int64_t Random::getNextRandomValue()
    {
        return m_random_distribution(m_random_generator);
    }
}}
