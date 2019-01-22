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

#include <KiwiEngine/KiwiEngine_Object.h>

#include <chrono>
#include <random>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                     RANDOM                                       //
    // ================================================================================ //
    
    class Random : public Object
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        Random(model::Object const& model, Patcher& patcher);
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
    private: // methods
        
        void setRange(int64_t range);
        void setSeed(int64_t new_seed);
        
        int64_t getNextRandomValue();
        
    private: // members
        
        using clock_t = std::chrono::high_resolution_clock;
        using rnd_distribution_t = std::uniform_int_distribution<int64_t>;
        
        clock_t::time_point m_start_time { clock_t::now() };
        std::mt19937        m_random_generator {};
        rnd_distribution_t  m_random_distribution {0ll, 0ll};
    };
    
}}
