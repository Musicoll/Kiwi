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
        
        static int64_t getUserId(model::Object const& model);
    
        //! @brief Get the compile options
        std::vector<std::string> getCompileOptions() const;
        
        //! @brief Set the DSP code
        void setCompileOptions(std::vector<std::string>&& options);
        
        //! @brief Get the DSP code
        std::string const& getDspCode() const;
        
        //! @brief Set the DSP code
        void setDspCode(std::string&& code);
        
        //! @brief Get the Edit code
        std::string const& getEditCode() const;
        
        //! @brief Set the Edit code
        void setEditCode(std::string&& code);
        
        //! @brief Get if the object has the lock for the codes
        bool hasLock() const;
        
        //! @brief Get if the object can grab the lock for the codes
        bool canLock() const;
        
        //! @brief Grab or leave the lock for the code
        //! @return true if the action succeed, otherwise false
        bool grabLock(bool state);
        
        //! @brief Force to unlock the model
        void forceUnlock();
        
        //! @brief Open a file an notify that the DSP and Edit codes changed
        void openFile(const std::string& file);
        
        //! @brief Compile the current DSP code.
        void compileDspCode();
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        struct nop
        {
            template <typename T>
            void operator() (T const &) const noexcept { }
        };
        
        class UIGlue;
        class FileSelector;
        class CodeEditor;
        typedef std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)> uptr_faust_factory;
        typedef std::unique_ptr<llvm_dsp, nop> uptr_faust_instance;
        
        uptr_faust_factory              m_factory;
        uptr_faust_instance             m_instance;
        std::mutex                      m_mutex;
        std::vector<dsp::sample_t*>     m_inputs;
        std::vector<dsp::sample_t*>     m_outputs;
        
        std::string                     m_dsp_code;
        std::string                     m_edit_code;
        std::vector<std::string>        m_compile_options;
        std::unique_ptr<UIGlue>         m_ui_glue;
        std::unique_ptr<FileSelector>   m_file_selector;
        std::unique_ptr<CodeEditor>     m_code_editor;
        int64_t                         m_lock_state = 0;
        const int64_t                   m_user_id;
    };

}}
