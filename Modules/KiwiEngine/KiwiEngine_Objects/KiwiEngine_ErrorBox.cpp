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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_ErrorBox.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                      ERRORBOX                                    //
    // ================================================================================ //
    
    void ErrorBox::declare()
    {
        Factory::add<ErrorBox>("errorbox", &ErrorBox::create);
    }
    
    std::unique_ptr<Object> ErrorBox::create(model::Object const& object, Patcher& patcher)
    {
        return std::make_unique<ErrorBox>(object, patcher);
    }
    
    ErrorBox::ErrorBox(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    {
        ;
    }
    
    void ErrorBox::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        ;
    }
    
    void ErrorBox::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        ;
    }
    
}}
