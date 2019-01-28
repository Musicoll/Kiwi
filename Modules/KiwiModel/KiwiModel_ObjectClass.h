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

#pragma once

#include <functional>
#include <set>
#include <string>

#include <flip/Class.h>
#include <flip/Mold.h>

#include <KiwiTool/KiwiTool_Parameter.h>
#include <KiwiTool/KiwiTool_Atom.h>

namespace kiwi { namespace model {
    
    class Object;
    
    class Factory;
    
    // ================================================================================ //
    //                                      PARAMETER CLASS                             //
    // ================================================================================ //
    
    //! @brief This is a parameter static informations.
    //! @details Attributes and Parameters share the same static representation.
    class ParameterClass
    {
    public: // classes
        
        enum class Type
        {
            Attribute, // Attributes are saved/collaborative but are not invariant.
            Parameter  // Parameters are not collaborative.
        };
        
    public: // methods
        
        //! @brief Constructor.
        ParameterClass(tool::Parameter::Type type);
        
        //! @brief Destructor.
        ~ParameterClass();
        
        //! @brief Sets the attributes type. Only used by factory.
        ParameterClass::Type getType() const;
        
        //! @brief Returns the parameter's data type.
        tool::Parameter::Type getDataType() const;
        
    private: // methods
        
        //! @brief Sets the attributes type. Only used by factory.
        void setType(Type param_type);
        
    private: // members
        
        Type                    m_type;
        tool::Parameter::Type   m_data_type;
        
        friend class ObjectClass;
        
    private: // deleted methods
        
        ParameterClass() = delete;
        ParameterClass(ParameterClass const& other) = delete;
        ParameterClass(ParameterClass && other) = delete;
        ParameterClass& operator=(ParameterClass const& other) = delete;
        ParameterClass& operator=(ParameterClass && other) = delete;
    };
    
    // ================================================================================ //
    //                                      OBJECTC LASS                                //
    // ================================================================================ //
    
    //! @brief The static representation of an object.
    class ObjectClass
    {
    public: // classes.
        
        //! @brief The construction method.
        using ctor_t = std::function<std::unique_ptr<model::Object>(std::vector<tool::Atom> const&)>;
        
        //! @brief A list of flags that defines the object's behavior.
        enum class Flag
        {
            Internal,       //! Internal objects do not appears in object list.
            DefinedSize     //! If the object has a predefined size.
        };
        
    private: // classes
        
        //! @brief The object's copy prototype.
        using mold_maker_t = std::function<void(model::Object const&, flip::Mold&)>;
        
        //! @brief The object paste prototype.
        using mold_caster_t = std::function<std::unique_ptr<model::Object>(flip::Mold const&)>;
        
    public:// methods
        
        //! @brief The ObjectClass constructor.
        //! @details Used in the object declaration.
        ObjectClass(std::string const& name, ctor_t ctor);
        
        //! @brief Destructor.
        ~ObjectClass();
        
        //! @brief Retrieves the object's class name.
        std::string const& getName() const;
        
        //! @brief Retrieves object's data model name.
        std::string const& getModelName() const;
        
        //! @brief Returns true if this object class has aliases.
        bool hasAlias() const noexcept;
        
        //! @brief Returns a set of aliases for this object.
        std::set<std::string> const& getAliases() const noexcept;
        
        //! @brief Returns true if this class has this alias name.
        bool hasAlias(std::string const& alias) const noexcept;
        
        //! @brief Adds a creator name alias to the class.
        void addAlias(std::string const& alias);
        
        //! @brief Adds an attribute to the class definition.
        void addAttribute(std::string const& name, std::unique_ptr<ParameterClass> param_class);
        
        //! @brief Returns true if attributes exists.
        bool hasAttribute(std::string const& name) const;
        
        //! @brief Returns an attributes static definition.
        //! @details Throws if no attributes refered by name.
        ParameterClass const& getAttribute(std::string const& name) const;
        
        //! @brief Adds a parameter to the class definition.
        void addParameter(std::string name, std::unique_ptr<ParameterClass> param_class);
        
        //! @brief Returns true if parameter exists.
        bool hasParameter(std::string const& name) const;
        
        //! @brief Returns a parameter's static definition.
        //! @details Throws if no parameter refered by name.
        ParameterClass const& getParameter(std::string const& name) const;
        
        //! @brief Gets the list of parameters static definition.
        std::map<std::string, std::unique_ptr<ParameterClass>> const& getParameters() const;
        
        //! @brief Adds the flag to object static definition.
        void setFlag(Flag const& flag);
        
        //! @brief Checks if the flag is set.
        bool hasFlag(Flag const& flag) const;
        
    private: // methods
        
        //! @brief Sets the mold maker function.
        void setMoldMaker(mold_maker_t maker);
        
        // @brief Sets the mold caster function.
        void setMoldCaster(mold_caster_t caster);
        
        //! @brief Sets the type_id's hash code of the class. Used by factory.
        void setTypeId(size_t);
        
        //! @brief Creates and returns a new Object with a vector of Atom as parameter.
        //! @param args A vector of Atom.
        //! @return A new model::Object.
        std::unique_ptr<model::Object> create(std::vector<tool::Atom> const& args) const;
        
        //! @brief Copy the content an object instance into a flip::Mold.
        //! @param object The object instance.
        //! @param mold The The flip::Mold where to copy object instance data.
        void moldMake(model::Object const& object, flip::Mold& mold) const;
        
        //! @brief Creates and returns a new Object from a flip::Mold.
        //! @param mold The The flip::Mold from which to retrieve object instance data.
        //! @return A new model::Object.
        std::unique_ptr<model::Object> moldCast(flip::Mold const& mold) const;
        
    private: // members
        
        std::string                                             m_name;
        std::string                                             m_model_name;
        std::set<std::string>                                   m_aliases;
        std::map<std::string, std::unique_ptr<ParameterClass>>  m_params;
        ctor_t                                                  m_ctor;
        std::set<Flag>                                          m_flags;
        mold_maker_t                                            m_mold_maker;
        mold_caster_t                                           m_mold_caster;
        size_t                                                  m_type_id;
        
        friend class Factory;
        
    private: // deleted methods.
        
        ObjectClass() = delete;
        ObjectClass(ObjectClass const& other) = delete;
        ObjectClass(ObjectClass && other) = delete;
        ObjectClass& operator=(ObjectClass const& other) = delete;
        ObjectClass& operator=(ObjectClass && other) = delete;
    };
    
}}
