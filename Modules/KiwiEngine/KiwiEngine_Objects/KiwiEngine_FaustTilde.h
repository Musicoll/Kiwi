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
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override;

        void attributeChanged(std::string const& name, tool::Parameter const& param) override;
    private:
        static std::string getName(model::Object const& model);
        static std::vector<std::string> getOptions(model::Object const& model);
        
        //! @brief Create the factory from a file
        //! @details Load and compile a DSP file in a parallel FAUST factory. If the compilation
        // succeeds, the engine changes its model using the code generated by the factory for the
        // DSP code member of model and the SHA key generated by the factory for the DSP name
        // attribute of model. Then, the engine waits for the attribute changed notification to
        // compile the code in the main FAUST factory.
        //! @see attributeChanged
        void openFile(const std::string& file);
        
        //! @brief Create the factory from a string
        //! @details Compile a code in the main factory.
        void createFactoryFromString(const std::string& name, const std::string& code);
        
        void createInstance();
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        struct nop
        {
            template <typename T>
            void operator() (T const &) const noexcept { }
        };
        
        class UIGlue;
        
        std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>  m_factory;
        std::unique_ptr<llvm_dsp, nop>      m_instance;
        std::mutex                          m_mutex;
        std::unique_ptr<UIGlue>             m_ui_glue;
        std::vector<dsp::sample_t*>         m_inputs;
        std::vector<dsp::sample_t*>         m_outputs;
        
        std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)> m_factory_engine;
        
        const std::vector<std::string>      m_options;
    };

}}
