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
    
    class FaustTilde::CodeEditor
    {
    public:
        
        CodeEditor(FaustTilde& o) : m_owner(o), m_manager(*this)
        {
            m_window.setContentNonOwned(&m_manager, true);
        }
        
        void open()
        {
            m_owner.deferMain([this]()
            {
                if(!m_window.isShowing())
                {
                    m_manager.setCode(m_owner.getDspCode());
                    m_window.addToDesktop();
                }
            });
        }
        
        void codeChanged()
        {
            m_owner.setEditCode(m_manager.getCode());
        }
        
        void synchronize()
        {
            m_owner.setDspCode(m_manager.getCode());
        }
        
    private:
        
        // ================================================================================ //
        
        class CodeEditorManager :
        public juce::Component,
        public juce::CodeDocument::Listener,
        public juce::ToolbarItemFactory,
        public juce::Button::Listener
        {
        public:
            
            CodeEditorManager(CodeEditor& o) : m_owner(o), m_editor(m_document, &m_highlither)
            {
                setBounds(0, 0, 512, 384);
                resized();
                m_editor.setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colours::white);
                m_editor.setScrollbarThickness(8);
                addAndMakeVisible(&m_editor);
                m_menu.setVertical(false);
                m_menu.setStyle(juce::Toolbar::textOnly);
                m_menu.addDefaultItems(*this);
                addAndMakeVisible(&m_menu);
            }
            
            void resized() override
            {
                const auto width = getWidth();
                const auto height = getHeight();
                m_editor.setBounds(0, 40, width, height - 40);
                m_menu.setBounds(0, 0, width, 40);
            }
            
            void visibilityChanged() override
            {
                if(isVisible())
                {
                    m_document.addListener(this);
                }
                else
                {
                    m_document.removeListener(this);
                }
            }
            
            void codeDocumentTextInserted(const juce::String& , int ) override
            {
                m_owner.codeChanged();
            }
            
            void codeDocumentTextDeleted(int , int ) override
            {
                m_owner.codeChanged();
            }
            
            void getAllToolbarItemIds(juce::Array <int>& ids) override
            {
                //, juce::ToolbarItemFactory::separatorBarId, 2, juce::ToolbarItemFactory::separatorBarId, 3};
                ids.add(1);
            }
            
            void getDefaultItemSet(juce::Array <int>& ids) override
            {
                ids.add(1);
            }
            
            juce::ToolbarItemComponent* createItem(int itemId) override
            {
                juce::String text;
                if(itemId == 1)
                {
                    text = "Sync";
                }
                auto btn = new juce::ToolbarButton(itemId, text, new juce::DrawableImage(), nullptr);
                btn->addListener(this);
                return btn;
            }
            
            void buttonClicked(juce::Button* btn) override
            {
                auto const iid = dynamic_cast<juce::ToolbarButton*>(btn)->getItemId();
                if(iid == 1)
                {
                    m_owner.synchronize();
                }
            }
            
            void setCode(std::string&& code)
            {
                m_editor.loadContent(juce::String(std::move(code)));
            }
            
            std::string getCode()
            {
                return m_document.getAllContent().toStdString();
            }
            
        private:
            CodeEditor&                 m_owner;
            juce::Toolbar               m_menu;
            FaustTokeniser              m_highlither;
            juce::CodeDocument          m_document;
            juce::CodeEditorComponent   m_editor;
        };
        
        // ================================================================================ //
        
        class Window : public juce::DocumentWindow
        {
        public:
            
            Window() :
            juce::DocumentWindow("FAUST Code Editor",
                                 juce::Colours::white,
                                 juce::DocumentWindow::allButtons,
                                 false)
            {
                setUsingNativeTitleBar(true);
                setResizable(true, true);
                setVisible(true);
                setTopLeftPosition(40, 40);
            }
            
            void closeButtonPressed() override
            {
                removeFromDesktop();
            }
        };
        
        // ================================================================================ //
        
        FaustTilde&       m_owner;
        Window            m_window;
        CodeEditorManager m_manager;
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
            bool        dirty; // If the parameter should be deleted
        };
        
        
        UIGlue(FaustTilde& owner) : m_owner(owner) {}
        
        void addButton(const char* label, FAUSTFLOAT* zone) final
        {
            addParameter(label, 0, zone, 0, 0, 0, 0);
        }
        
        void addCheckButton(const char* label, FAUSTFLOAT* zone) final
        {
            addParameter(label, 1, zone, 0.f, 1.f, 1.f, 0.f);
        }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            addParameter(label, 2, zone, min, max, step, init);
        }
        
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            addParameter(label, 2, zone, min, max, step, init);
        }
        
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            addParameter(label, 2, zone, min, max, step, init);
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
        
        void log()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            m_owner.log("faust~: number of parameters " + std::to_string(m_parameters.size()));
            for(auto const& param : m_parameters)
            {
                m_owner.log(" ");
                m_owner.log("faust~: parameter " + param.first);
                m_owner.log("faust~: type " + std::to_string(param.second.type));
                m_owner.log("faust~: default " + std::to_string(param.second.saved));
                m_owner.log("faust~: minimum " + std::to_string(param.second.min));
                m_owner.log("faust~: maximum " + std::to_string(param.second.max));
                m_owner.log("faust~: step " + std::to_string(param.second.step));
            }
        }
        
        void set(const std::string& name)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_parameters.find(name);
                if(it != m_parameters.end())
                {
                    if(it->second.dirty)
                    {
                        m_mutex_glue.unlock();
                        m_owner.warning(std::string("FAUST interfaces \"") + name + std::string("\" not valid anymore"));
                        return;
                    }
                    if(it->second.type == 0)
                    {
                        const auto cvalue   = *(it->second.zone);
                        *(it->second.zone) = static_cast<FAUSTFLOAT>(cvalue < std::numeric_limits<FAUSTFLOAT>::epsilon());
                    }
                    else
                    {
                        m_owner.warning(std::string("FAUST interface \"") + name + std::string("\" is a requires a value"));
                    }
                }
                else
                {
                    m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                }
                m_mutex_glue.unlock();
            }
            else
            {
                m_owner.warning(std::string("FAUST interfaces being processed - please wait"));
            }
        }
        
        void set(const std::string& name, FAUSTFLOAT value)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_parameters.find(name);
                if(it != m_parameters.end())
                {
                    if(it->second.dirty)
                    {
                        m_mutex_glue.unlock();
                        m_owner.warning(std::string("FAUST interfaces \"") + name + std::string("\" not valid anymore"));
                        return;
                    }
                    if(it->second.type == 1)
                    {
                        *(it->second.zone) = static_cast<FAUSTFLOAT>(value < std::numeric_limits<FAUSTFLOAT>::epsilon());
                    }
                    else if(it->second.type == 2)
                    {
                        const FAUSTFLOAT min = it->second.min;
                        const FAUSTFLOAT max = it->second.max;
                        *(it->second.zone) = std::max(std::min(max, value), min);
                    }
                    else
                    {
                        m_owner.warning(std::string("FAUST interface \"") + name + std::string("\" is doesn't requires a value"));
                    }
                }
                else
                {
                    m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                }
                m_mutex_glue.unlock();
            }
            else
            {
                m_owner.warning(std::string("FAUST interfaces being processed - please wait"));
            }
        }
        
        void saveStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                param.second.saved = *param.second.zone;
            }
        }
        
        void recallStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                *param.second.zone = param.second.saved;
            }
        }
        
        void prepareChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                param.second.dirty = true;
            }
        }
        
        void finishChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                if(param.second.dirty)
                {
                    m_parameters.erase(param.first);
                }
            }
        }
        
    private:
        
        void addParameter(const char* name, int type, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step, FAUSTFLOAT init)
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            auto it = m_parameters.find(name);
            if(it != m_parameters.end())
            {
                if(it->second.type != type ||
                   it->second.min != min ||
                   it->second.max != max ||
                   it->second.step != step)
                {
                    it->second = Parameter({type, zone, min, max, step, init, false});
                    *zone = init;
                }
                else
                {
                    it->second.saved = *it->second.zone;
                    it->second.zone = zone;
                    it->second.dirty = false;
                    *zone = it->second.saved;
                }
            }
            else
            {
                m_parameters[name] = Parameter({type, zone, min, max, step, init, false});
                *zone = init;
            }
        }
        
        std::map<std::string, Parameter> m_parameters;
        FaustTilde&                      m_owner;
        std::mutex                       m_mutex_glue;
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
        attributeChanged("dspcodechanged", {tool::Parameter::Type::String, {std::string("")}});
    }
    
    FaustTilde::~FaustTilde()
    {
        
    }
    
    // ================================================================================ //
    
    std::string FaustTilde::getDspCode() const
    {
        return m_dsp_code;
    }
    
    void FaustTilde::setDspCode(std::string&& code)
    {
        deferMain([this, ncode = std::move(code)]()
                  {
                      auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                      if(model)
                      {
                          model->setDSPCode(ncode);
                          setAttribute(std::string("dspcodechanged"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
                      }
                  });
    }
    
    std::string FaustTilde::getEditCode() const
    {
        return m_edit_code;
    }
    
    void FaustTilde::setEditCode(std::string&& code)
    {
        deferMain([this, ncode = std::move(code)]()
                  {
                      auto* model = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                      if(model)
                      {
                          model->setEditCode(ncode);
                          setAttribute(std::string("editcodechanged"), {tool::Parameter::Type::String, {juce::Uuid().toString().toStdString()}});
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
        setDspCode(jf.loadFileAsString().toStdString());
    }
    
    // ================================================================================ //
    
    void FaustTilde::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "dspcodechanged")
        {
            auto const value = parameter[0].getString();
            deferMain([this, value]()
                      {
                          auto* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                          if(fmodel)
                          {
                              auto code = fmodel->getDSPCode();
                              compileCode(value, code);
                              m_dsp_code.swap(code);
                          }
                      });
        }
        else if(name == "editcodechanged")
        {
            auto const value = parameter[0].getString();
            deferMain([this, value]()
                      {
                          auto* fmodel = dynamic_cast<model::FaustTilde*>(&getObjectModel());
                          if(fmodel)
                          {
                              auto code = fmodel->getEditCode();
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
        
        uptr_faust_factory nfactory(nullptr, deleteDSPFactory);
        if(startMTDSPFactories())
        {
            nfactory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromString(name, code, m_options.size(), argv.data(), std::string(), errors), deleteDSPFactory);
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
        if(nfactory)
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
        }
        else
        {
            m_ui_glue->prepareChanges();
            {
                // Safetly release the instance
                std::lock_guard<std::mutex> guard(m_mutex);
                m_instance.reset();
            }
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
                setEditCode(std::string(m_dsp_code));
                m_code_editor->open();
                if(args.size() > 1)
                {
                    warning(std::string("faust~: editor method extra arguments"));
                }
                return;
            }
            if(!m_factory || !m_instance)
            {
                return;
            }
            try
            {
                if(args.size() == 1)
                {
                    m_ui_glue->set(name);
                }
                else if(args.size() > 1)
                {
                    if(args[1].isNumber())
                    {
                        m_ui_glue->set(name, args[1].getFloat());
                        if(args.size() > 2)
                        {
                            warning(std::string("faust~: FAUST interface \"") + name + std::string("\" too many arguments"));
                        }
                    }
                    else
                    {
                        warning(std::string("faust~: FAUST interface \"") + name + std::string("\" requires a number"));
                    }
                }
            }
            catch (std::string& e)
            {
                warning(std::string("faust~: ")  + e);
            }
        }
        else
        {
            warning(std::string("faust~: receive bad arguments"));
        }
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
                m_ui_glue->saveStates();
                m_instance->instanceInit(static_cast<int>(infos.sample_rate));
                m_ui_glue->recallStates();
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
