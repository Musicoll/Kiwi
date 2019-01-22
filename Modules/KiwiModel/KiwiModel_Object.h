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

//! @todo Clean flip headers below, use only needed one in this file

// ---- Flip headers ---- //
#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/String.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"
#include "flip/Enum.h"
#include "flip/Signal.h"

#include <KiwiTool/KiwiTool_Atom.h>
#include <KiwiTool/KiwiTool_Parameter.h>
#include <KiwiTool/KiwiTool_Listeners.h>

#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_Error.h>

#include <mutex>
#include <algorithm>
#include <exception>
#include <set>
#include <map>

namespace kiwi
{
    namespace model
    {
        class Factory;
        
        // ================================================================================ //
        //                                  INLET/OUTLET                                    //
        // ================================================================================ //
  
        //! @brief Class that represent a type of pin.
        //! @details This class is a flip object wrapper on an enum. It's needed for putting the type
        //! into a flip::Array.
        class PinType : public flip::Object
        {
        public: // classes
            
            enum class IType
            {
                Control,
                Signal
            };
            
        public: // methods
            
            // @brief Constructor of type.
            PinType(IType type);
            
            //! @brief Comprison operator.
            bool operator<(PinType const& other) const;
            
            //! @brief Equality comparator. Consistent with comparison operator.
            bool operator==(PinType const& other) const;
            
        public: // internal methods
            
            //! @internal Flip default constructor.
            PinType(flip::Default&);
            
            //! @internal Flip declarator.
            static void declare();
            
        private: // methods
            
            //! @brief Returns the type or the previous type if the Type is deleted.
            //! @details During document changed phase the type can be tagged as removed.
            IType getType() const;
            
        private:
            flip::Enum<IType> m_type;
        };
        
        //! @brief Class that represent an inlet able to have multiple types.
        class Inlet : public flip::Object
        {
        public:
            //! @brief Initializes the Inlet with multiple types.
            Inlet(std::set<PinType> types);
            
            //! @brief The destructor.
            ~Inlet() = default;
            
            //! @brief Checks if the inlet is compatible  with type.
            bool hasType(PinType type) const;
            
        public: // internal methods
            
            //! @internal Flip default constructor.
            Inlet(flip::Default&);
            
            //! @internal Flip declarator.
            static void declare();
            
        private:
            flip::Array<PinType> m_types;
        };
        
        //! @brief Class that represent a certain outlet having only one type.
        class Outlet : public flip::Object
        {
        public:
            //! @brief Initializes the Inlet with one type.
            Outlet(PinType type);
            
            //! @brief The destructor.
            ~Outlet() = default;
            
            // @brief Returns the type of the outlet.
            PinType const& getType() const;
            
        public: // internal methods
            
            //! @internal Flip default constructor.
            Outlet(flip::Default&);
            
            //! @internal Flip declarator.
            static void declare();
            
        private:
            PinType m_type;
        };
        
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is a base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public flip::Object
        {
        public: // classes
            
            using SignalKey = uint32_t;
            
            class Listener
            {
            public:
                
                //! @brief Destructor
                virtual ~Listener() = default;
                
                //! @brief Called when a parameter has changed;
                virtual void modelParameterChanged(std::string const& name, tool::Parameter const& param) =  0;
                
                //! @brief Called when an attribute has changed.
                virtual void modelAttributeChanged(std::string const& name, tool::Parameter const& param) = 0;
            };
            
            //! @brief an error that object can throw to notify a problem.
            //! @todo Check if object's id shall be added to error.
            class Error : public model::Error
            {
            public: // methods
                
                //! @brief Constructor.
                Error(std::string const& message):model::Error(message) {}
                
                //! @brief Destructor.
                ~Error() = default;
            };
            
        public: // methods
 
            //! @brief Constructor.
            Object();
            
            //! @brief Destructor.
            virtual ~Object() = default;
            
            //! @brief Returns the arguments of the object.
            std::vector<tool::Atom> const& getArguments() const;
            
            //! @brief Returns a list of changed attributes.
            //! @details Use this function in the observation to check which values shall
            //! be updated.
            std::set<std::string> getChangedAttributes() const;
            
            //! @brief Retrieve one of the object's attributes.
            tool::Parameter const& getAttribute(std::string const& name) const;
            
            //! @brief Sets one of the object's attribute.
            void setAttribute(std::string const& name, tool::Parameter const& param);
            
            //! @brief Returns one of the object's parameters.
            tool::Parameter const& getParameter(std::string const& name) const;
            
            //! @brief Sets one of the object's parameter.
            void setParameter(std::string const& name, tool::Parameter const& param);
            
            //! @brief Writes the parameter into data model.
            //! @details If the parameter is saved this function will be called at every attempt to
            //! modify the parameter. Never called for non saved parameters.
            virtual void writeAttribute(std::string const& name, tool::Parameter const& paramter);
            
            //! @brief Reads the model to initialize a parameter.
            //! @details Saved parameters may infos from the data model.
            virtual void readAttribute(std::string const& name, tool::Parameter & parameter) const;
            
            //! @brief Checks the data model to see if a parameter has changed.
            //! @details Only called for saved parameters. Default returns false.
            virtual bool attributeChanged(std::string const& name) const;
            
            //! @brief Adds a listener of object's parameters.
            void addListener(Listener& listener) const;
            
            //! @brief Removes listenere from list.
            void removeListener(Listener& listener) const;
            
            //! @brief Returns the name of the Object.
            std::string getName() const;
            
            //! @brief Returns the object's static definition.
            ObjectClass const& getClass() const;
            
            //! @brief Returns the text of the Object.
            std::string getText() const;
            
            //! @brief Returns the inlets of the Object.
            flip::Array<Inlet> const& getInlets() const;
            
            //! @brief Returns the inlets at index
            Inlet const& getInlet(size_t index) const;
            
            //! @brief Returns the number of inlets.
            size_t getNumberOfInlets() const;
            
            //! @brief Returns true if the inlets changed.
            bool inletsChanged() const noexcept;
            
            //! @brief Returns the number of outlets.
            flip::Array<Outlet> const& getOutlets() const;
            
            //! @brief Returns the outlets at corresponding index.
            Outlet const& getOutlet(size_t index) const;
            
            //! @brief Returns the number of outlets.
            size_t getNumberOfOutlets() const;
            
            //! @brief Returns true if the outlets changed.
            bool outletsChanged() const noexcept;
            
            //! @brief Sets the x/y graphical position of the object.
            void setPosition(double x, double y);
            
            //! @brief Returns true if the object's position changed.
            bool positionChanged() const noexcept;
            
            //! @brief Returns true if the object's size changed.
            bool sizeChanged() const noexcept;
            
            //! @brief Returns true if the position or the size of the object changed.
            bool boundsChanged() const noexcept;
            
            //! @brief Returns the x position.
            double getX() const noexcept;
            
            //! @brief Returns the y position.
            double getY() const noexcept;
            
            //! @brief Sets the width of the object.
            void setWidth(double new_width);
            
            //! @brief Sets the height of the object.
            void setHeight(double new_height);
            
            //! @brief Returns the object's width.
            double getWidth() const noexcept;
            
            //! @brief Returns the object's height.
            double getHeight() const noexcept;
            
            //! @brief Returns inlet or outlet description.
            virtual std::string getIODescription(bool is_inlet, size_t index) const;
            
            //! @brief Checks if the object has this flag set.
            bool hasFlag(ObjectClass::Flag flag) const;
            
            //! @brief Returns the object's signal referenced by this key.
            //! @details Throws an exception if no signal is referenced for key.
            template <class... Args>
            auto& getSignal(SignalKey key) const
            {
                flip::SignalBase& signal_base = *m_signals.at(key);
                return dynamic_cast<flip::Signal<Args...>&>(signal_base);
            }
            
        protected:
            
            //! @brief Adds a signal having singal key.
            template <class... Args>
            void addSignal(SignalKey key, model::Object& object)
            {
                m_signals.emplace(key, std::make_unique<flip::Signal<Args...>>(key, object));
            }
            
            //! @brief Clear and replace all the object's inlets.
            void setInlets(flip::Array<Inlet> const& inlets);
            
            //! @brief Clear and replace all the object's outlets.
            void setOutlets(flip::Array<Outlet> const& outlets);
            
            //! @brief Adds an inlet at end of current inlet list.
            void pushInlet(std::set<PinType> type);
            
            //! @brief Adds an outlet at end of current outlet list.
            void pushOutlet(PinType type);
                
        public: // internal methods
            
            //! @internal flip Default constructor
            Object(flip::Default&);
            
            //! @internal flip static declare method
            static void declare();
            
        private: // members
            
            flip::String                                            m_text;
            flip::Array<Inlet>                                      m_inlets;
            flip::Array<Outlet>                                     m_outlets;
            flip::Float                                             m_position_x;
            flip::Float                                             m_position_y;
            flip::Float                                             m_width;
            flip::Float                                             m_height;
            mutable std::map<std::string, tool::Parameter>          m_attributes;
            mutable std::map<std::string, tool::Parameter>          m_parameters;
            mutable std::unique_ptr<std::vector<tool::Atom>>        m_args = nullptr;
            std::map<SignalKey, std::unique_ptr<flip::SignalBase>>  m_signals;
            mutable tool::Listeners<Listener>                       m_listeners;
            
            friend class Factory;
        
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(model::Object&&) = delete;
            model::Object& operator=(model::Object const&) = delete;
            model::Object& operator=(model::Object&&) = delete;
        };
    }
}
