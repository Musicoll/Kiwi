/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_MODEL_FACTORY_HPP_INCLUDED
#define KIWI_MODEL_FACTORY_HPP_INCLUDED

#include "KiwiModel_Object.hpp"
#include "KiwiModel_DataModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   OBJECT FACTORY                                 //
        // ================================================================================ //
        
        //! @brief The model Object's factory
        class Factory
        {
        public: // classes
            
            class ObjectClassBase;
            template<class T> class ObjectClass;
            
        public: // methods
            
            //! @brief isValidObject type traits
            //! @details To be considered as valid an object must:
            //! - not be abstract.
            //! - inherit from model::Object.
            //! - be constructible with flip::Default.
            //! - be constructible with a string and a vector of atoms.
            template<typename TModel> struct isValidObject
            {
                enum
                {
                    value = std::is_base_of<model::Object, TModel>::value
                    && !std::is_abstract<TModel>::value
                    && std::is_constructible<TModel, std::string, std::vector<Atom>>::value
                    && std::is_constructible<TModel, flip::Default &>::value
                };
            };
            
            //! @brief Adds an object model into the Factory.
            //! @details The function throw if the object has already been added.
            //! Calling this function will imply a change of datamodel.
            //! @param name The name of the object.
            template<class TModel>
            static ObjectClass<TModel>& add(std::string const& name)
            {
                static_assert(isValidObject<TModel>::value, "Not a valid Object");
                
                assert((!name.empty() && !DataModel::has<TModel>())
                       && "Object name empty or object class already added");
                
                // check if the name match the name of another object in the factory.
                if(has(name))
                {
                    throw std::runtime_error("The \"" + name + "\" object is already in the factory");
                }

                auto& object_classes = getClasses();
                const auto it = object_classes.emplace(object_classes.end(),
                                                       std::make_unique<ObjectClass<TModel>>(name));
                
                return dynamic_cast<ObjectClass<TModel>&>(*(it->get()));
            }
            
            //! @brief Creates a new Object with a name and arguments.
            //! @details This function will throw if the object name does not exist.
            //! @param name The name of the Object.
            //! @param args A list of arguments as a vector of Atom.
            //! @return A ptr to a model::Object.
            static std::unique_ptr<model::Object> create(std::string const& name,
                                                         std::vector<Atom> const& args);
            
            //! @brief Creates a new Object from a flip::Mold.
            //! @details This function will throw if the object name does not exist.
            //! @param name The name of the Object to create.
            //! @param mold The flip::Mold from which to create the object.
            //! @return A ptr to a model::Object.
            static std::unique_ptr<model::Object> create(std::string const& name,
                                                         flip::Mold const& mold);
            
            //! @brief Make a mold of this object.
            //! @details This function will throw if the object does not exist.
            //! @param object The Object to copy.
            //! @param mold The flip::Mold from which to create the object.
            //! @return A ptr to a model::Object.
            static void copy(model::Object const& object, flip::Mold& mold);
            
            //! @brief Returns a ptr to an object class thas has this name or alias name.
            //! @details This method returns nullptr if the name is not found.
            //! @param name The name or an alias name of the class.
            //! @param ignore_aliases Default to false, pass true to ignore aliases.
            //! @return A ptr to an ObjectClassBase.
            static ObjectClassBase* getClassByName(std::string const& name,
                                                   const bool ignore_aliases = false);
            
            //! @brief Returns true if a given string match a registered object class name.
            //! @param name The name of the object class to find.
            //! @return true if the object class has been added, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Gets the names of the objects that has been added to the Factory.
            //! @param ignore_aliases Default to false, you may pass true to exclude them.
            //! @param ignore_internals Default to true, you may pass false to include them.
            //! @return A vector of object class names.
            static std::vector<std::string> getNames(const bool ignore_aliases = false,
                                                     const bool ignore_internals = true);
            
        private: // methods
            
            using object_classes_t = std::vector<std::unique_ptr<ObjectClassBase>>;
            
            using ctor_fn_t = std::function<std::unique_ptr<model::Object>(std::vector<Atom>)>;
            using mold_maker_fn_t = std::function<void(model::Object const&, flip::Mold&)>;
            using mold_caster_fn_t = std::function<std::unique_ptr<model::Object>(flip::Mold const&)>;
            
            //! @internal Returns a constructor function.
            template<class TModel>
            static ctor_fn_t getCtor(std::string const& name)
            {
                return [name](std::vector<Atom> const& args) -> std::unique_ptr<model::Object>
                {
                    return std::make_unique<TModel>(name, args);
                };
            }
            
            //! @internal Returns a mold maker function.
            template<class TModel>
            static mold_maker_fn_t getMoldMaker()
            {
                return [](model::Object const& object, flip::Mold& mold) -> void
                {
                    // make a mold with container_flag active
                    mold.make(static_cast<TModel const&>(object), false);
                };
            }
            
            //! @internal Returns a mold caster function.
            template<class TModel>
            static mold_caster_fn_t getMoldCaster()
            {
                return [](flip::Mold const& mold)
                {
                    flip::Default d;
                    auto object_uptr = std::make_unique<TModel>(d);
                    mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
                    return object_uptr;
                };
            }
            
            //! @internal Returns a sanitized version of the string.
            static std::string sanitizeName(std::string const& name);
            
            //! @internal Returns the object classes.
            static object_classes_t& getClasses();
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
        
        // ================================================================================ //
        //                             FACTORY OBJECT CLASS BASE                            //
        // ================================================================================ //
        
        //! @brief ObjectClass base class
        class Factory::ObjectClassBase
        {
        public: // methods
            
            //! @brief Constructor.
            ObjectClassBase(std::string const& name,
                            std::string const& model_name,
                            const ctor_fn_t ctor,
                            const mold_maker_fn_t mold_maker,
                            const mold_caster_fn_t  mold_caster)
            :
            m_name(name),
            m_model_name(model_name),
            m_ctor(ctor),
            m_mold_maker(mold_maker),
            m_mold_caster(mold_caster) {}
            
            //! @brief Destructor.
            virtual ~ObjectClassBase() = default;
            
            //! @brief Returns the name of the object.
            std::string const& getName() const { return m_name; }
            
            //! @brief Returns the name used into the data model of kiwi.
            std::string const& getModelName() const { return m_model_name; }
            
            //! @brief Returns true if it's an internal object.
            bool isInternal() const noexcept { return m_internal; }
            
            //! @brief Pass true if this is an internal object.
            void setInternal(const bool is_internal) noexcept { m_internal = is_internal; }
            
            //! @brief Returns true if this object class has aliases.
            bool hasAlias() const noexcept { return !m_aliases.empty(); }
            
            //! @brief Pass true if this is an internal object.
            std::set<std::string> const& getAliases() const noexcept { return m_aliases; }
            
            //! @brief Returns true if this class has this alias name.
            bool hasAlias(std::string const& alias) const noexcept { return (m_aliases.count(alias) != 0); }
            
            //! @brief Adds a creator name alias to the class.
            void addAlias(std::string alias)
            {
                if(!Factory::has(alias))
                {
                    m_aliases.emplace(std::move(alias));
                }
            }
            
            //! @brief Returns a new Object by mold cast.
            std::unique_ptr<model::Object> create(std::vector<Atom> const& args) const
            { return m_ctor(args); }
            
            //! @brief Returns a new Object by mold cast.
            void moldMake(model::Object const& object, flip::Mold& mold) const
            { m_mold_maker(object, mold); }
            
            //! @brief Returns a new Object by mold cast.
            std::unique_ptr<model::Object> moldCast(flip::Mold const& mold) const
            { return m_mold_caster(mold); }
            
        private: // members
            
            const std::string       m_name {};
            const std::string       m_model_name {};
            std::set<std::string>   m_aliases {};
            const ctor_fn_t         m_ctor {};
            const mold_maker_fn_t   m_mold_maker {};
            const mold_caster_fn_t  m_mold_caster {};
            bool                    m_internal = false;
        };
        
        // ================================================================================ //
        //                               FACTORY OBJECT CLASS                               //
        // ================================================================================ //
        
        //! @brief ObjectClass
        template<class TObjectClass>
        class Factory::ObjectClass : public ObjectClassBase
        {
        public: // methods
            
            using class_t = TObjectClass;
            
            ObjectClass(std::string const& name)
            : ObjectClassBase(name,
                              Factory::sanitizeName(name),
                              getCtor<class_t>(name),
                              getMoldMaker<class_t>(),
                              getMoldCaster<class_t>()),
            m_flip_class(DataModel::declare<class_t>())
            {
                m_flip_class.name(getModelName().c_str())
                .template inherit<model::Object>();
            }
            
            //! @brief Add a flip member to the ObjectClass.
            template<class U, U class_t::*ptr_to_member>
            void addMember(char const* name)
            {
                m_flip_class.template member(name);
            }

        private: // members
            flip::Class<class_t>& m_flip_class;
        };
    }
}

#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
