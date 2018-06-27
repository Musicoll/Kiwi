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
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustCodeTokenizer.h>
#include <KiwiEngine/KiwiEngine_Factory.h>
#include <faust/dsp/llvm-dsp.h>
#include <faust/gui/UI.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_FaustTilde.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    class FaustTilde::Window : public juce::DocumentWindow
    {
    public:
        Window() : juce::DocumentWindow("FAUST Editor", juce::Colours::white, juce::DocumentWindow::allButtons, false)
        {
            setUsingNativeTitleBar(true);
            setResizable(true, true);
            setVisible(true);
        }
        
        void closeButtonPressed() override
        {
            removeFromDesktop();
        }
    };
    
    // ================================================================================ //
    
    class FaustTilde::FileSelector
    {
    public:
        
        FileSelector(FaustTilde& o) : owner(o) {}
        
        void open()
        {
            auto directory = juce::File(last_directory);
            if(!directory.exists())
            {
                directory = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
            }
            fc = std::make_unique<juce::FileChooser>("Choose a DSP file to read...",
                                                     directory,
                                                     juce::String("*.dsp"), true);
            owner.deferMain([this]() {
                
                this->fc->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, [this](juce::FileChooser const& chooser)
                                      {
                                          auto file = chooser.getResult();
                                          if(file.getFullPathName().isNotEmpty())
                                          {
                                              this->owner.openFile(file.getFullPathName().toStdString());
                                              this->last_directory = file.getParentDirectory().getFullPathName().toStdString();
                                          }
                                      });
            });
        }
        
    private:
        
        FaustTilde& owner;
        std::unique_ptr<juce::FileChooser> fc;
        std::string last_directory;
    };
    
    // ================================================================================ //
    
    class FaustTilde::CodeEditor : public juce::CodeDocument::Listener
    {
    public:
        
        CodeEditor(FaustTilde& o) :
        owner(o),
        window(),
        document(),
        highlither(),
        editor(document, &highlither)
        {
            editor.setBounds(0, 0, 512, 384);
            editor.setVisible(true);
            editor.setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colours::white);
            editor.setScrollbarThickness(8);
            window.setContentNonOwned(&editor, true);
        }
        
        void open()
        {
            owner.deferMain([this]()
            {
                if(!window.isShowing())
                {
                    document.removeListener(this);
                    editor.loadContent(juce::String(owner.getCode()));
                    window.addToDesktop();
                    document.addListener(this);
                }
            });
        }
        
        void codeDocumentTextInserted(const String& , int ) override
        {
            owner.setCode(document.getAllContent().toStdString());
        }
        
        void codeDocumentTextDeleted(int , int ) override
        {
            owner.setCode(document.getAllContent().toStdString());
        }
        
    private:
        
        FaustTilde&               owner;
        Window                    window;
        juce::CodeDocument        document;
        FaustTokeniser            highlither;
        juce::CodeEditorComponent editor;
    };
    
    // ================================================================================ //
    
    class FaustTilde::UIGlue : public UI
    {
    public:
        class Parameter
        {
        public:
            int         type;
            FAUSTFLOAT* zone;
            FAUSTFLOAT  min;
            FAUSTFLOAT  max;
            FAUSTFLOAT  step;
            FAUSTFLOAT  saved;
        };
        
        std::map<std::string, Parameter> m_parameters;
        FaustTilde&                      m_owner;
        
        UIGlue(FaustTilde& owner) : m_owner(owner) {}
        
        void addButton(const char* label, FAUSTFLOAT* zone) final
        {
            m_parameters[label] = Parameter({0, zone, 0, 0, 0, 0});
        }
        
        void addCheckButton(const char* label, FAUSTFLOAT* zone) final
        {
             m_parameters[label] = Parameter({1, zone, 0.f, 1.f, 1.f, 0.f});
        }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, min, max, step, init});
        }
        
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, min, max, step, init});
        }
        
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, min, max, step, init});
        }
        
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) final {};
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) final {};
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) final {}
        
        void declare(FAUSTFLOAT*, const char* key, const char* value) final
        {
            m_owner.log(std::string("faust~: declare ") + std::string(key) + std::string(" - ") + std::string(value));
        }
        
        void openTabBox(const char* label) final {};
        void openHorizontalBox(const char* label) final {}
        void openVerticalBox(const char* label) final {}
        void closeBox() final {}
    };
    
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
    
    std::string FaustTilde::getName(model::Object const& model)
    {
        return model.getArguments()[2].getString() + std::string(".dsp");
    }
    
    std::vector<std::string> FaustTilde::getOptions(model::Object const& model)
    {
        std::vector<std::string> options;
        auto const& args = model.getArguments();
        for(size_t i = 3; i < args.size(); ++i)
        {
            options.push_back(tool::AtomHelper::toString(args[i], false));
        }
        if(!std::count(options.begin(), options.end(), "-I"))
        {
            auto const file = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile);
            options.push_back("-I");
            options.push_back(file.getFullPathName().toStdString() + std::string("/Contents/Resources/Faust/Libs"));
        }
        return options;
    }
    

    FaustTilde::FaustTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    m_factory(nullptr, deleteDSPFactory),
    m_options(getOptions(model)),
    m_ui_glue(std::make_unique<UIGlue>(*this)),
    m_file_selector(std::make_unique<FileSelector>(*this)),
    m_code_editor(std::make_unique<CodeEditor>(*this))
    {
        attributeChanged("compiled", {tool::Parameter::Type::String, {std::string("")}});
    }
    
    FaustTilde::~FaustTilde()
    {
        
    }
    
    // ================================================================================ //
    
    std::string FaustTilde::getCode() const
    {
        return m_code;
    }
    
    void FaustTilde::setCode(std::string&& code)
    {
        deferMain([this, ncode = std::move(code)]()
                  {
                      auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                      if(model)
                      {
                          model->setDSPCode(ncode);
                          setAttribute(std::string("compiled"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
                      }
                  });
    }
    
    void FaustTilde::openFile(const std::string& file)
    {
        juce::File const jf(file);
        if(!jf.exists())
        {
            warning("faust~: " + file + " doesn't exist");
            return;
        }
        if(!jf.hasFileExtension(".dsp"))
        {
            warning("faust~: " + file + " is not a FAUST DSP file");
            return;
        }
        setCode(jf.loadFileAsString().toStdString());
    }
    
    // ================================================================================ //
    
    void FaustTilde::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "compiled")
        {
            auto const value = parameter[0].getString();
            deferMain([this, value]()
                      {
                          auto* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                          if(fmodel)
                          {
                              auto code = fmodel->getDSPCode();
                              compileCode(value, code);
                              m_code.swap(code);
                          }
                      });
        }
    }
    
    void FaustTilde::compileCode(const std::string& name, const std::string& code)
    {
        std::string errors;
        std::vector<char const*> argv(m_options.size());
        for(size_t i = 0; i < m_options.size(); ++i)
        {
            argv[i] = m_options[i].c_str();
        }
        auto nfactory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromString(name, code, m_options.size(), argv.data(), std::string(), errors), deleteDSPFactory);
        
        if(!errors.empty())
        {
            warning("faust~: compilation failed - " + errors);
        }
        if(nfactory)
        {
            log("faust~: compilation succeed - " + nfactory->getName());
            auto ninstance = std::unique_ptr<llvm_dsp, nop>(nfactory->createDSPInstance());
            // Safetly swap the factory
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                m_instance = std::move(ninstance);
            }
            if(!m_instance)
            {
                m_ui_glue = std::make_unique<UIGlue>(*this);
                warning("faust~: DSP allocation failed");
            }
            else
            {
                auto nglue = std::make_unique<UIGlue>(*this);
                m_instance->buildUserInterface(nglue.get());
                m_ui_glue = std::move(nglue);
                log("faust~: DSP allocation succeed");
                log("faust~: number of inputs " + std::to_string(m_instance->getNumInputs()));
                log("faust~: number of outputs " + std::to_string(m_instance->getNumOutputs()));
                log("faust~: number of parameters " + std::to_string(m_ui_glue->m_parameters.size()));
                for(auto const& param : m_ui_glue->m_parameters)
                {
                    log(" ");
                    log("faust~: parameter " + param.first);
                    log("faust~: type " + std::to_string(param.second.type));
                    log("faust~: default " + std::to_string(param.second.saved));
                    log("faust~: minimum " + std::to_string(param.second.min));
                    log("faust~: maximum " + std::to_string(param.second.max));
                    log("faust~: step " + std::to_string(param.second.step));
                }
            }
        }
        else
        {
            m_ui_glue = std::make_unique<UIGlue>(*this);
            // Safetly release the instance
            std::lock_guard<std::mutex> guard(m_mutex);
            m_instance.reset();
        }
        m_factory = std::move(nfactory);
    }
    
    void FaustTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!args.empty() && args[0].isString())
        {
            const auto& name = args[0].getString();
            if(name == "open")
            {
                if(args.size() == 1)
                {
                    m_file_selector->open();
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
            else if(name == "editor")
            {
                m_code_editor->open();
                if(args.size() > 1)
                {
                    warning(std::string("faust~: editor method extra arguments"));
                }
                return;
            }
            for(auto const& param : m_ui_glue->m_parameters)
            {
                if(param.first == name)
                {
                    if(param.second.type == 0)
                    {
                        *param.second.zone = static_cast<FAUSTFLOAT>(!(*param.second.zone != 0.f));
                        return;
                    }
                    if(param.second.type == 1)
                    {
                        if(args.size() > 1 && args[1].isNumber())
                        {
                            *param.second.zone = static_cast<FAUSTFLOAT>(args[1].getFloat()) != 0.f;
                            if(args.size() > 2)
                            {
                                warning(std::string("faust~: receive method - interface \"")  + name + std::string("\" too many arguments"));
                            }
                            return;
                        }
                        warning(std::string("faust~: receive method - interface \"") + name + std::string("\" expects a number"));
                    }
                    else
                    {
                        if(args.size() > 1 && args[1].isNumber())
                        {
                            *param.second.zone = std::max(std::min(static_cast<FAUSTFLOAT>(args[1].getFloat()), param.second.max), param.second.min);
                            if(args.size() > 2)
                            {
                                warning(std::string("faust~: receive method - interface \"")  + name + std::string("\" too many arguments"));
                            }
                            return;
                        }
                        warning(std::string("faust~: receive method - interface \"") + name + std::string("\" expects a number"));
                    }
                    
                }
            }
            warning(std::string("faust~: receive method - no such interface \"") + name + std::string("\""));
            return;
        }
        warning(std::string("faust~: receive method - bad argument"));
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
    
    void FaustTilde::prepare(PrepareInfo const& infos)
    {
        if(m_instance)
        {
            if(static_cast<size_t>(m_instance->getNumInputs()) < getNumberOfInputs() &&
               static_cast<size_t>(m_instance->getNumOutputs()) < getNumberOfOutputs())
            {
                for(auto& param : m_ui_glue->m_parameters)
                {
                    param.second.saved = *param.second.zone;
                }
                m_instance->instanceInit(static_cast<int>(infos.sample_rate));
                for(auto& param : m_ui_glue->m_parameters)
                {
                    *param.second.zone = param.second.saved;
                }
                m_inputs.resize(m_instance->getNumInputs());
                m_outputs.resize(m_instance->getNumOutputs());
                setPerformCallBack(this, &FaustTilde::perform);
            }
            else
            {
                warning("faust~: DSP instance has invalid number of inputs and outputs");
            }
        }
    }
    
}}
