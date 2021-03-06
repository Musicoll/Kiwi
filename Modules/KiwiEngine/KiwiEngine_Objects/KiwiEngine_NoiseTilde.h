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

#include <random>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                     NOISE~                                       //
    // ================================================================================ //
    
    class NoiseTilde : public AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        NoiseTilde(model::Object const& model, Patcher& patcher);
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        
    private: // members
        
        std::random_device                              m_random_devive;
        std::mt19937                                    m_random_generator;
        std::uniform_real_distribution<dsp::sample_t>   m_random_distribution;
    };
    
}}
