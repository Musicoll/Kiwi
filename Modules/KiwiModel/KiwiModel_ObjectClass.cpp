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

#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_Object.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      PARAMETER CLASS                             //
    // ================================================================================ //
    
    ParameterClass::ParameterClass(tool::Parameter::Type type):
    m_type(),
    m_data_type(type)
    {
    }
    
    ParameterClass::~ParameterClass()
    {
    }
    
    tool::Parameter::Type ParameterClass::getDataType() const
    {
        return m_data_type;
    }
    
    void ParameterClass::setType(Type param_type)
    {
        m_type = param_type;
    }
    
    ParameterClass::Type ParameterClass::getType() const
    {
        return m_type;
    }
    
    
    // ================================================================================ //
    //                                     OBJECT CLASS                                 //
    // ================================================================================ //
    
    ObjectClass::ObjectClass(std::string const& name, ctor_t ctor):
    m_name(name),
    m_model_name(Factory::toModelName(name)),
    m_aliases(),
    m_params(),
    m_ctor(ctor),
    m_flags(),
    m_mold_maker(),
    m_mold_caster(),
    m_type_id()
    {
    }
    
    ObjectClass::~ObjectClass()
    {
    }
    
    bool ObjectClass::hasAlias() const noexcept
    {
        return !m_aliases.empty();
    }
    
    std::string const& ObjectClass::getName() const
    {
        return m_name;
    }
    
    std::string const& ObjectClass::getModelName() const
    {
        return m_model_name;
    }
    
    std::set<std::string> const& ObjectClass::getAliases() const noexcept
    {
        return m_aliases;
    }
    
    bool ObjectClass::hasAlias(std::string const& alias) const noexcept
    {
        return (m_aliases.count(alias) != 0);
    }
    
    void ObjectClass::addAlias(std::string const& alias)
    {
        m_aliases.insert(alias);
    }
    
    void ObjectClass::addAttribute(std::string const& name, std::unique_ptr<ParameterClass> param_class)
    {
        assert(param_class != nullptr);
        
        param_class->setType(ParameterClass::Type::Attribute);
        
        m_params[name] = std::move(param_class);
    }
    
    bool ObjectClass::hasAttribute(std::string const& name) const
    {
        auto found_param = m_params.find(name);
        
        return found_param != m_params.end()
               && found_param->second->getType() == ParameterClass::Type::Attribute;
    }
    
    ParameterClass const& ObjectClass::getAttribute(std::string const& name) const
    {
        return *m_params.at(name);
    }
    
    void ObjectClass::addParameter(std::string name, std::unique_ptr<ParameterClass> param_class)
    {
        assert(param_class != nullptr);
        
        param_class->setType(ParameterClass::Type::Parameter);
        
        m_params[name] = std::move(param_class);
    }
    
    bool ObjectClass::hasParameter(std::string const& name) const
    {
        auto found_param = m_params.find(name);
        
        return found_param != m_params.end()
               && found_param->second->getType() == ParameterClass::Type::Parameter;
    }
    
    ParameterClass const& ObjectClass::getParameter(std::string const& name) const
    {
        return *m_params.at(name);
    }
    
    std::map<std::string, std::unique_ptr<ParameterClass>> const& ObjectClass::getParameters() const
    {
        return m_params;
    }
    
    void ObjectClass::setFlag(Flag const& flag)
    {
        m_flags.insert(flag);
    }
    
    bool ObjectClass::hasFlag(Flag const& flag) const
    {
        return m_flags.find(flag) != m_flags.end();
    }
    
    void ObjectClass::setMoldMaker(mold_maker_t maker)
    {
        m_mold_maker = maker;
    }
    
    void ObjectClass::setMoldCaster(mold_caster_t caster)
    {
        m_mold_caster = caster;
    }
    
    void ObjectClass::setTypeId(size_t type_id)
    {
        m_type_id = type_id;
    }
    
    std::unique_ptr<model::Object> ObjectClass::create(std::vector<tool::Atom> const& args) const
    {
        return m_ctor(args);
    }
    
    void ObjectClass::moldMake(model::Object const& object, flip::Mold& mold) const
    {
        m_mold_maker(object, mold);
    }
    std::unique_ptr<model::Object> ObjectClass::moldCast(flip::Mold const& mold) const
    {
        return m_mold_caster(mold);
    }
    
}}
