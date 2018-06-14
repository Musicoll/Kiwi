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
#include <faust/gui/UI.h>

namespace kiwi { namespace engine {
    
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
             m_parameters[label] = Parameter({1, zone,0, 0, 1, 1});
        }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, init, min, max, step});
        }
        
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, init, min, max, step});
        }
        
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) final
        {
            m_parameters[label] = Parameter({2, zone, init, min, max, step});
        }
        
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) final {};
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) final {};
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) final {}
        
        void declare(FAUSTFLOAT*, const char* key, const char* value) final
        {
            m_owner.log(std::string(key) + std::string(": ") + std::string(value));
        }
        
        void openTabBox(const char* label) final {};
        void openHorizontalBox(const char* label) final {}
        void openVerticalBox(const char* label) final {}
        void closeBox() final {}
    };
    
    // ================================================================================ //
    //                                       FAUST~                                     //
    // ================================================================================ //
    
    std::string const FaustTilde::m_folder = std::string("/Users/pierre/Documents/Kiwi Media/Faust/");
    
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
            options.push_back("-I");
            options.push_back(m_folder + std::string("libs"));
        }
        return options;
    }
    

    
    FaustTilde::FaustTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    m_factory(nullptr, deleteDSPFactory),
    m_ui_glue(std::make_unique<UIGlue>(*this)),
    m_name(getName(model)),
    m_options(getOptions(model))
    {
        createFactory();
        if(m_factory)
        {
            createInstance();
        }
    }
    
    FaustTilde::~FaustTilde()
    {
        
    }
    
    void FaustTilde::createFactory()
    {
        std::string errors;
        char const* argv[m_options.size()];
        warning(m_folder + m_name);
        for(size_t i = 0; i < m_options.size(); ++i)
        {
            argv[i] = m_options[i].c_str();
            warning(m_options[i]);
        }
        m_ui_glue->m_parameters.clear();
        m_factory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromFile(m_folder + m_name, m_options.size(), argv, std::string(), errors), deleteDSPFactory);
        if(!errors.empty())
        {
            warning(errors);
        }
    }
    
    void FaustTilde::createInstance()
    {
        assert(m_factory && "factory must not be null");
        m_ui_glue->m_parameters.clear();
        m_instance = std::unique_ptr<llvm_dsp, nop>(m_factory->createDSPInstance());
        if(!m_instance)
        {
            warning("faust~: can't allocate DSP instance");
            return;
        }
        m_instance->buildUserInterface(m_ui_glue.get());
    }
    
    void FaustTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!args.empty() && args[0].isString())
        {
            const auto& name = args[0].getString();
            for(auto const& param : m_ui_glue->m_parameters)
            {
                if(param.first == name)
                {
                    if(param.second.type == 0)
                    {
                        *param.second.zone = !(*param.second.zone != 0.f);
                        return;
                    }
                    if(param.second.type == 1)
                    {
                        if(args.size() > 1 && args[1].isNumber())
                        {
                            *param.second.zone = args[1].getFloat() != 0.f;
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
                            float value = args[1].getFloat();
                            value = value > param.second.min ? value : param.second.min;
                            value = value < param.second.max ? value : param.second.max;
                            *param.second.zone = args[1].getFloat() != 0.f;
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
        for(size_t i = 0; i < ninputs; ++i)
        {
            m_inputs[i] = const_cast<dsp::sample_t*>(input[i].data());
        }
        for(size_t i = 0; i < noutputs; ++i)
        {
            m_outputs[i] = output[i].data();
        }
        m_instance->compute(nsamples, m_inputs.data(), m_outputs.data());
    }
    
    void FaustTilde::prepare(PrepareInfo const& infos)
    {
        if(m_instance)
        {
            if(static_cast<size_t>(m_instance->getNumInputs()) < getNumberOfInputs() &&
               static_cast<size_t>(m_instance->getNumOutputs()) < getNumberOfOutputs())
            {
                m_instance->instanceInit(static_cast<int>(infos.sample_rate));
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
