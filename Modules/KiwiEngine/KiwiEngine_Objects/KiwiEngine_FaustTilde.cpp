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

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       PLUS~                                      //
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
    AudioObject(model, patcher), m_name(getName(model)), m_options(getOptions(model))
    {
        loadFactory();
    }
    
    FaustTilde::~FaustTilde()
    {
        deleteInstance();
        deleteFactory();
    }
    
    void FaustTilde::deleteInstance()
    {
        if(m_instance)
        {
            delete m_instance;
            m_instance = NULL;
        }
    }
    
    void FaustTilde::deleteFactory()
    {
        if(m_factory)
        {
            assert(m_instance == nullptr && "instance must be null");
            deleteDSPFactory(m_factory);
            m_factory = NULL;
        }
    }
    
    void FaustTilde::loadFactory()
    {
        std::string errors;
        char const* argv[m_options.size()];
        warning(m_folder + m_name);
        for(size_t i = 0; i < m_options.size(); ++i)
        {
            argv[i] = m_options[i].c_str();
            warning(m_options[i]);
        }
        
        m_factory = createDSPFactoryFromFile(m_folder + m_name, m_options.size(), argv, std::string(), errors);
        if(!errors.empty())
        {
            warning(errors);
        }
    }
    
    void FaustTilde::prepare(PrepareInfo const& infos)
    {
        
    }
    
}}
