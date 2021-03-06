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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_FaustTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT FAUST~                                   //
    // ================================================================================ //
    
    void FaustTilde::iString::declare()
    {
        DataModel::declare<FaustTilde::iString>()
        .name("faust_istring")
        .member<flip::String, &FaustTilde::iString::value>("value");
    }
    
    FaustTilde::iString::iString(flip::Default& d)
    {
        value.disable_in_undo();
    }
    
    FaustTilde::iString::iString(flip::String&& v) : value(v)
    {
        value.disable_in_undo();
    }
    
    FaustTilde::iString::iString(std::string const& v) : value(v)
    {
        value.disable_in_undo();
    }
    
    void FaustTilde::declare()
    {
        std::unique_ptr<ObjectClass> fausttilde_class(new ObjectClass("faust~", &FaustTilde::create));
        
        auto param_dsp_code_changed = std::make_unique<ParameterClass>(tool::Parameter::Type::String);
        fausttilde_class->addAttribute("dspcodechanged", std::move(param_dsp_code_changed));
        
        auto param_edit_code_changed = std::make_unique<ParameterClass>(tool::Parameter::Type::String);
        fausttilde_class->addAttribute("editcodechanged", std::move(param_edit_code_changed));
        
        auto param_lock_state = std::make_unique<ParameterClass>(tool::Parameter::Type::Int);
        fausttilde_class->addAttribute("lockstate", std::move(param_lock_state));
        
        auto param_compile_options = std::make_unique<ParameterClass>(tool::Parameter::Type::String);
        fausttilde_class->addAttribute("compileoptionschanged", std::move(param_compile_options));
        
        iString::declare();
        
        flip::Class<FaustTilde> & fausttilde_model = DataModel::declare<FaustTilde>()
        .name(fausttilde_class->getModelName().c_str())
        .inherit<Object>()
        .member<flip::String, &FaustTilde::m_dsp_code>("dspcode")
        .member<flip::Message<std::string>, &FaustTilde::m_dsp_code_changed>("dspcodechanged")
        .member<flip::String, &FaustTilde::m_edit_code>("editcode")
        .member<flip::Message<std::string>, &FaustTilde::m_edit_code_changed>("editcodechanged")
        .member<flip::Int, &FaustTilde::m_lock_state>("lockstate")
        .member<flip::Array<iString>, &FaustTilde::m_compiles_options>("compileoptions")
        .member<flip::Message<std::string>, &FaustTilde::m_compiles_options_changed>("compileoptionschanged");
        
        Factory::add<FaustTilde>(std::move(fausttilde_class), fausttilde_model);
    }
    
    std::unique_ptr<Object> FaustTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<FaustTilde>(args);
    }
    
    FaustTilde::FaustTilde(flip::Default& d) : model::Object(d)
    {
        m_dsp_code.disable_in_undo();
        m_dsp_code_changed.disable_in_undo();
        m_edit_code.disable_in_undo();
        m_edit_code_changed.disable_in_undo();
        m_lock_state.disable_in_undo();
        m_compiles_options.disable_in_undo();
        m_compiles_options_changed.disable_in_undo();
    }
    
    FaustTilde::FaustTilde(std::vector<tool::Atom> const& args)
    {
        m_dsp_code.disable_in_undo();
        m_dsp_code_changed.disable_in_undo();
        m_edit_code.disable_in_undo();
        m_edit_code_changed.disable_in_undo();
        m_lock_state.disable_in_undo();
        m_compiles_options.disable_in_undo();
        m_compiles_options_changed.disable_in_undo();
        if (args.size() < 2)
        {
            throw Error("faust~ expects 2 default arguments: the number of inlets and the number of outlets.");
        }
        
        if (!args[0].isInt() || args[0].getInt() < 0)
        {
            throw Error("faust~ 1st argument must be the number of inlets - positive or null");
        }
        if (!args[1].isInt() || args[1].getInt() < 0)
        {
            throw Error("faust~ 2nd argument must be the number of outlets - positive or null");
        }
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        for(int i = 1; i < args[0].getInt(); ++i)
        {
            pushInlet({PinType::IType::Signal});
        }
        for(int i = 0; i < args[1].getInt(); ++i)
        {
            pushOutlet(PinType::IType::Signal);
        }
        pushOutlet(PinType::IType::Control);
    }
    
    std::string FaustTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return std::string("Messages and Audio Input Channel ") + std::to_string(index);
            }
            return std::string("Audio Input Channel ") + std::to_string(index);
        }
        else
        {
            if(index == getNumberOfOutlets() -  1)
            {
                return std::string("Extra Output");
            }
            return std::string("Audio Output Channel ") + std::to_string(index);
        }
    }
    
    void FaustTilde::setDSPCode(std::string const& newcode)
    {
        m_dsp_code = newcode;
    }
    
    std::string FaustTilde::getDSPCode() const
    {
        return m_dsp_code;
    }
    
    void FaustTilde::setEditCode(std::string const& newcode)
    {
        m_edit_code = newcode;
    }
    
    std::string FaustTilde::getEditCode() const
    {
        return m_edit_code;
    }
    
    int64_t FaustTilde::getLockState() const
    {
        return m_lock_state;
    }
    
    void FaustTilde::setCompileOptions(std::vector<std::string> const& newcompileoptions)
    {
        m_compiles_options.clear();
        for(auto const& option : newcompileoptions)
        {
            m_compiles_options.emplace(m_compiles_options.end(), flip::String(option));
        }
    }
    
    std::vector<std::string> FaustTilde::getCompileOptions() const
    {
        std::vector<std::string> options;
        auto it = m_compiles_options.begin();
        while(it != m_compiles_options.end())
        {
            options.push_back((*it).value);
            ++it;
        }
        return options;
    }
    
    void FaustTilde::writeAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        if(name == "dspcodechanged")
        {
            m_dsp_code_changed.send(parameter[0].getString());
        }
        else if(name == "editcodechanged")
        {
            m_edit_code_changed.send(parameter[0].getString());
        }
        else if(name == "lockstate")
        {
            m_lock_state = parameter[0].getInt();
        }
        else if(name == "compileoptionschanged")
        {
            m_compiles_options_changed.send(parameter[0].getString());
        }
    }
    
    void FaustTilde::readAttribute(std::string const& name, tool::Parameter & parameter) const
    {
        if(name == "dspcodechanged")
        {
            m_dsp_code_changed.get([&parameter](bool forward_flag, std::string received_message)
                                   {
                                       parameter = tool::Parameter(tool::Parameter::Type::String, {received_message});
                                   });
        }
        else if(name == "editcodechanged")
        {
            m_edit_code_changed.get([&parameter](bool forward_flag, std::string received_message)
                                   {
                                       parameter = tool::Parameter(tool::Parameter::Type::String, {received_message});
                                   });
        }
        else if(name == "lockstate")
        {
            parameter = tool::Parameter(tool::Parameter::Type::Int, {static_cast<int64_t>(m_lock_state)});
        }
        else if(name == "compileoptionschanged")
        {
            m_compiles_options_changed.get([&parameter](bool forward_flag, std::string received_message)
                                    {
                                        parameter = tool::Parameter(tool::Parameter::Type::String, {received_message});
                                    });
        }
    }
    
    bool FaustTilde::attributeChanged(std::string const& name) const
    {
        if(name == "dspcodechanged")
        {
            return m_dsp_code_changed.changed();
        }
        else if(name == "editcodechanged")
        {
            return m_edit_code_changed.changed();
        }
        else if(name == "lockstate")
        {
            return m_lock_state.changed();
        }
        else if(name == "compileoptionschanged")
        {
            return m_compiles_options_changed.changed();
        }
        return false;
    }
    
}}
