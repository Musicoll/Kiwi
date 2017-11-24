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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_GreaterEqualTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       >=~                                         //
    // ================================================================================ //
    
    void GreaterEqualTilde::declare()
    {
        Factory::add<GreaterEqualTilde>(">=~", &GreaterEqualTilde::create);
    }
    
    std::unique_ptr<Object> GreaterEqualTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<GreaterEqualTilde>(model, patcher);
    }
    
    GreaterEqualTilde::GreaterEqualTilde(model::Object const& model, Patcher& patcher):
    OperatorTilde(model, patcher)
    {
    }
    
    void GreaterEqualTilde::compute(dsp::sample_t & result, dsp::sample_t const& lhs, dsp::sample_t const& rhs)
    {
        result = lhs >= rhs ? 1 : 0;
    }
    
}}
