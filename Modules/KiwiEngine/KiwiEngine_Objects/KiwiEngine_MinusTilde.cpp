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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_MinusTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       -~                                         //
    // ================================================================================ //
    
    void MinusTilde::declare()
    {
        Factory::add<MinusTilde>("-~", &MinusTilde::create);
    }
    
    std::unique_ptr<Object> MinusTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<MinusTilde>(model, patcher);
    }
    
    MinusTilde::MinusTilde(model::Object const& model, Patcher& patcher):
    OperatorTilde(model, patcher)
    {
    }
    
    void MinusTilde::compute(dsp::sample_t & result, dsp::sample_t const& lhs, dsp::sample_t const& rhs)
    {
        result = lhs - rhs;
    }
    
}}
