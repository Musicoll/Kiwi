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

#pragma once

#include <KiwiEngine/KiwiEngine_Object.h>

class llvm_dsp_factory;
class llvm_dsp;

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       FAUST~                                     //
    // ================================================================================ //
    
    class FaustTilde : public engine::AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        FaustTilde(model::Object const& model, Patcher& patcher);
        
        ~FaustTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) final {}
        
        void prepare(dsp::Processor::PrepareInfo const& infos) final;
        
    private:
        
        static std::string getName(model::Object const& model);
        static std::vector<std::string> getOptions(model::Object const& model);
        
        void deleteInstance();
        void deleteFactory();
        void loadFactory();
        
        const std::string m_name;
        const std::vector<std::string> m_options;
        llvm_dsp_factory* m_factory  = nullptr;
        llvm_dsp*         m_instance = nullptr;
        
        static std::string const m_folder;
    };

}}
