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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>
#include <faust/dsp/llvm-dsp.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_FaustTilde.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>

#include "KiwiEngine_FaustTilde/KiwiEngine_FaustTilde_CodeEditor.cpp"
#include "KiwiEngine_FaustTilde/KiwiEngine_FaustTilde_UIGLue.cpp"
#include "KiwiEngine_FaustTilde/KiwiEngine_FaustTilde_FileSelector.cpp"

namespace kiwi { namespace engine {

    // ================================================================================ //
    //                                       FAUST~                                     //
    // ================================================================================ //

    void FaustTilde::declare()
    {
        Factory::add<FaustTilde>("faust~", &FaustTilde::create);
    }
    
    std::unique_ptr<Object> FaustTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<FaustTilde>(model, patcher);
    }

    FaustTilde::FaustTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    , m_factory(nullptr, deleteDSPFactory)
    , m_compile_options({"-I", getFaustLibsPath()})
    , m_ui_glue(std::make_unique<UIGlue>(*this))
    , m_file_selector(std::make_unique<FileSelector>(*this))
    , m_code_editor(std::make_unique<CodeEditor>(*this))
    , m_user_id(getUserId(model))
    {
        attributeChanged("dspcodechanged", {tool::Parameter::Type::String, {std::string("")}});
        attributeChanged("editcodechanged", {tool::Parameter::Type::String, {std::string("")}});
        attributeChanged("compileoptionschanged", {tool::Parameter::Type::String, {std::string("")}});
        
        if(auto const* fmodel = dynamic_cast<model::FaustTilde const*>(&model))
        {
            attributeChanged("lockstate", {tool::Parameter::Type::Int, {fmodel->getLockState()}});
        }
    }

    FaustTilde::~FaustTilde()
    {}

    // ================================================================================ //
    bool FaustTilde::grabLock(bool state)
    {
        if(!hasLock() && !canLock() && state)
        {
            warning("The object is already locked");
            return false;
        }
        else if(hasLock() && !state)
        {
            setAttribute(std::string("lockstate"), {tool::Parameter::Type::Int, {0}});
        }
        else if(canLock() && state)
        {
            setAttribute(std::string("lockstate"), {tool::Parameter::Type::Int, {m_user_id}});
        }
        return true;
    }

    bool FaustTilde::hasLock() const
    {
        return m_lock_state == m_user_id;
    }

    bool FaustTilde::canLock() const
    {
        return m_lock_state == 0;
    }

    void FaustTilde::forceUnlock()
    {
        setAttribute(std::string("lockstate"), {tool::Parameter::Type::Int, {0}});
    }
    
    std::string FaustTilde::getFaustLibsPath() const
    {
        const auto appfile = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile);
        
#ifdef __APPLE__
        return appfile.getChildFile("Contents/Resources/libs/faust").getFullPathName().toStdString();
#elif _WIN32
        auto apppath = appfile.getParentDirectory().getFullPathName().toStdString();
        std::replace(apppath.begin(), apppath.end(), '\\', '/');
        return apppath + std::string("/libs/faust");
#else
        return appfile.getParentDirectory().getChildFile("libs/faust").getFullPathName().toStdString();
#endif
    }
    
    int64_t FaustTilde::getUserId(model::Object const& model)
    {
        int64_t temp;
        uint64_t userid = model.document().user();
        std::memcpy(&temp, &userid, sizeof(temp));
        return temp;
    }

    std::vector<std::string> FaustTilde::getCompileOptions() const
    {
        return m_compile_options;
    }

    void FaustTilde::setCompileOptions(std::vector<std::string>&& options)
    {
        if(!canLock() && !hasLock())
        {
            warning("faust~: code is currently locked by another user");
            return;
        }
        
        deferMain([this, noptions = std::move(options)]() {
            if(auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
            {
                model->setCompileOptions(noptions);
                setAttribute(std::string("compileoptionschanged"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
            }
        });
    }

    std::string const& FaustTilde::getDspCode() const
    {
        return m_dsp_code;
    }

    void FaustTilde::setDspCode(std::string&& code)
    {
        if(!canLock() && !hasLock())
        {
            warning("faust~: code is currently locked by another user");
            return;
        }
        
        deferMain([this, ncode = std::move(code)]() {
            if(auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
            {
                model->setDSPCode(ncode);
                setAttribute(std::string("dspcodechanged"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
            }
        });
    }

    std::string const& FaustTilde::getEditCode() const
    {
        return m_edit_code;
    }

    void FaustTilde::setEditCode(std::string&& code)
    {
        if(!canLock() && !hasLock())
        {
            warning("faust~: code is currently locked by another user");
            return;
        }
        
        deferMain([this, ncode = std::move(code)]() {
            if(auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
            {
                model->setEditCode(ncode);
                setAttribute(std::string("editcodechanged"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
            }
        });
    }

    void FaustTilde::openFile(std::string const& filepath)
    {
        juce::File const file(filepath);
        if(!file.exists())
        {
            warning("faust~: " + filepath + " doesn't exist");
            return;
        }
        
        if(!file.hasFileExtension(".dsp"))
        {
            warning("faust~: " + filepath + " is not a FAUST DSP file");
            return;
        }
        
        const auto file_as_string = file.loadFileAsString();
        setEditCode(file_as_string.toStdString());
        setDspCode(file_as_string.toStdString());
    }
    
    void FaustTilde::compileDspCode()
    {
        if(m_dsp_code.empty())
        {
            m_ui_glue->prepareChanges();
            {
                // Safetly release the instance
                std::lock_guard<std::mutex> guard(m_mutex);
                m_instance.reset();
            }
            
            m_factory.reset();
            return;
        }
        
        std::string errors;
        uptr_faust_factory nfactory(nullptr, deleteDSPFactory);
        if(startMTDSPFactories())
        {
            const auto options = m_compile_options;
            const auto name = "kiwi" + juce::Uuid().toString().toStdString();
            std::vector<char const*> argv(options.size());
            for(size_t i = 0; i < options.size(); ++i)
            {
                argv[i] = options[i].c_str();
            }
            
            nfactory = std::unique_ptr<llvm_dsp_factory, decltype(&deleteDSPFactory)>(createDSPFactoryFromString(name, m_dsp_code, options.size(), argv.data(), std::string(), errors), deleteDSPFactory);
            stopMTDSPFactories();
        }
        else
        {
            warning("faust~: can't start multi-thread access");
        }

        if(!errors.empty())
        {
            warning("faust~: compilation failed - " + errors);
        }
        else if(nfactory)
        {
            log("faust~: compilation succeed - " + nfactory->getName());
            auto ninstance = std::unique_ptr<llvm_dsp, nop>(nfactory->createDSPInstance());

            if(ninstance)
            {
                m_ui_glue->prepareChanges();
                // Safetly swap the factory
                {
                    std::lock_guard<std::mutex> guard(m_mutex);
                    m_instance = std::move(ninstance);
                }
                m_instance->buildUserInterface(m_ui_glue.get());
                m_ui_glue->finishChanges();
                log("faust~: DSP allocation succeed");
                log("faust~: number of inputs " + std::to_string(m_instance->getNumInputs()));
                log("faust~: number of outputs " + std::to_string(m_instance->getNumOutputs()));
                m_ui_glue->log();
                prepareDsp(m_sample_rate, m_block_size);
            }
            else
            {
                m_ui_glue->prepareChanges();
                warning("faust~: DSP allocation failed");
                {
                    // Safetly release the instance
                    std::lock_guard<std::mutex> guard(m_mutex);
                    m_instance.reset();
                }
            }
            
            m_factory = std::move(nfactory);
            return;
        }
        
        m_ui_glue->prepareChanges();
        {
            // Safetly release the instance
            std::lock_guard<std::mutex> guard(m_mutex);
            m_instance.reset();
        }
        m_factory.reset();
    }

    // ================================================================================ //

    void FaustTilde::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "dspcodechanged")
        {
            deferMain([this, value = parameter[0].getString()]() {
                if(auto* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
                {
                    m_dsp_code = fmodel->getDSPCode();
                    compileDspCode();
                }
            });
        }
        else if(name == "editcodechanged")
        {
            deferMain([this]() {
                if(auto* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
                {
                    m_edit_code = fmodel->getEditCode();
                    m_code_editor->downloadCode();
                }
            });
        }
        else if(name == "compileoptionschanged")
        {
            deferMain([this]() {
                if(auto const* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel()))
                {
                    auto noptions = fmodel->getCompileOptions();
                    m_compile_options.resize(2 + noptions.size());
                    for(size_t i = 0; i < noptions.size(); ++i)
                    {
                        m_compile_options[i+2].swap(noptions[i]);
                    }
                    m_code_editor->updateCompileOptions();
                    compileDspCode();
                }
            });
        }
        else if(name == "lockstate")
        {
            m_lock_state = parameter[0].getInt();
            m_code_editor->updateLockState();
        }
    }
    // The Kiwi Object interface
    // ================================================================================ //

    void FaustTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!args.empty() && args[0].isString())
        {
            const auto& name = args[0].getString();
            if(name == "open")
            {
                if(args.size() == 1)
                {
                    m_file_selector->show();
                }
                else
                {
                    if(args[1].isString())
                    {
                        openFile(args[1].getString());
                    }
                    else
                    {
                        warning(std::string("faust~: open method expects a path"));
                    }
                    
                    if(args.size() > 2)
                    {
                        warning(std::string("faust~: open method extra arguments"));
                    }
                }
                
                return;
            }
            
            if(name == "editor")
            {
                deferMain([this]() {
                    std::string ncode = m_dsp_code;
                    setEditCode(std::move(ncode));
                    m_code_editor->show();
                });

                if(args.size() > 1)
                {
                    warning(std::string("faust~: editor method extra arguments"));
                }
                
                return;
            }
            
            if(name == "options")
            {
                std::vector<std::string> noptions(args.size() - 1);
                for(size_t i = 1; i < args.size(); ++i)
                {
                    if(args[i].isNumber())
                        noptions[i-1] = std::to_string(args[i].getFloat());
                    else
                        noptions[i-1] = args[i].getString();
                }
                setCompileOptions(std::move(noptions));
                return;
            }

            if(!m_factory || !m_instance)
            {
                return;
            }

            if(m_ui_glue->hasOutput(name))
            {
                send(getNumberOfOutputs() - 1, {m_ui_glue->getOutput(name)});
                return;
            }
            
            if(args.size() == 1)
            {
                m_ui_glue->setInput(name, 0);
                return;
            }
            
            if(args[1].isNumber())
            {
                m_ui_glue->setInput(name, args[1].getFloat());
                if(args.size() > 2)
                {
                    warning(std::string("faust~: FAUST interface \"") + name + std::string("\" too many arguments"));
                }
                return;
            }
            
            warning(std::string("faust~: FAUST interface \"") + name + std::string("\" wrong arguments"));
            return;
        }
        
        warning(std::string("faust~: receive bad arguments"));
    }

    void FaustTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        const size_t nsamples = input.getVectorSize();
        const size_t ninputs  = m_inputs.size();
        const size_t noutputs = m_outputs.size();
        if(m_mutex.try_lock())
        {
            if(m_instance)
            {
                for(size_t i = 0; i < ninputs; ++i)
                {
                    m_inputs[i] = const_cast<dsp::sample_t*>(input[i].data());
                }
                for(size_t i = 0; i < noutputs; ++i)
                {
                    m_outputs[i] = output[i].data();
                }
                m_instance->compute(nsamples, const_cast<FAUSTFLOAT**>(m_inputs.data()), m_outputs.data());
            }
            else
            {
                for(size_t i = ninputs; i < noutputs; ++i)
                {
                    std::fill_n(output[i].data(), nsamples, 0.f);
                }
            }
            m_mutex.unlock();
        }
        else
        {
            for(size_t i = ninputs; i < noutputs; ++i)
            {
                std::fill_n(output[i].data(), nsamples, 0.f);
            }
        }
    }

    bool FaustTilde::prepareDsp(size_t sampleRate, size_t blockSize) noexcept
    {
        m_sample_rate   = sampleRate;
        m_block_size    = blockSize;

        if(static_cast<size_t>(m_instance->getNumInputs()) <= getNumberOfInputs() &&
           static_cast<size_t>(m_instance->getNumOutputs()) <= getNumberOfOutputs() - 1)
        {
            m_ui_glue->saveStates();
            m_instance->instanceInit(static_cast<int>(m_sample_rate));
            m_ui_glue->recallStates();
            m_inputs.resize(m_instance->getNumInputs());
            m_outputs.resize(m_instance->getNumOutputs());
            return true;
        }
        return false;
    }

    void FaustTilde::prepare(PrepareInfo const& infos)
    {
        if(m_instance)
        {
            if(prepareDsp(infos.sample_rate, infos.vector_size))
            {
                setPerformCallBack(this, &FaustTilde::perform);
            }
            else
            {
                warning("faust~: DSP instance has invalid number of inputs and outputs, expected at least " + std::to_string(m_instance->getNumInputs()) + " inputs but has " + std::to_string(getNumberOfInputs()) + " and " + std::to_string(m_instance->getNumOutputs()) + " inputs but has " + std::to_string(getNumberOfOutputs() - 1));
            }
        }
    }

}}
