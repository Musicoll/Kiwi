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

#include <mutex>
#include <algorithm>
#include <exception>
#include <set>

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
        
        //! @brief Class that represents an object's property.
        //! @details Adding, removing new flags is a data model change and needs migration.
        class Flag : public flip::Object
        {
        public: // classes
            
            //! @brief The internal enum representation of the flag.
            enum class IFlag
            {
                DefinedSize = 0,    // Initial object's Size defined by the model.
                ResizeWidth,        // Object is resizable horizontally.
                ResizeHeight        // Object is resizable vertically.
            };
            
        public: // methods
            
            //! @brief Flag constructor.
            Flag(Flag::IFlag flag);
            
            //! @brief Flip default constructor.
            Flag(flip::Default& d);
            
            //! @brief Checks if flags are equals.
            bool operator==(Flag const& other) const;
            
            //! @brief Checks if flags are differents.
            bool operator!=(Flag const& other) const;
            
            //! @internal Flip declarator.
            static void declare();
            
        private: // members
            
            flip::Enum<IFlag> m_flag;
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
            
        public: // methods
 
            //! @brief Constructor.
            Object();
            
            //! @brief Destructor.
            virtual ~Object() = default;
            
            //! @brief Returns the name of the Object.
            std::string getName() const;
            
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
            //! @details Width will not be lower than minimal width.
            //! If ratio was previously set proportions will be kept intact by changing height.
            void setWidth(double new_width);
            
            //! @brief Sets the height of the object.
            //! @details Height will not be lower than minimal height.
            //! If ratio was previously set proportions will be kept intact by changing width.
            void setHeight(double new_height);
                
            //! @brief Returns the aspect ratio.
            double getRatio() const;
            
            //! @brief Returns the object's width.
            double getWidth() const noexcept;
            
            //! @brief Returns the object's height.
            double getHeight() const noexcept;
            
            //! @brief Returns the minimal width for this object.
            double getMinWidth() const noexcept;
                
            //! @brief Returns the minimal height for this object;
            double getMinHeight() const noexcept;
            
            //! @brief Returns inlet or outlet description.
            virtual std::string getIODescription(bool is_inlet, size_t index) const;
            
            //! @brief Returns the object's signal referenced by this key.
            //! @details Throws an exception if no signal is referenced for key.
            template <class... Args>
            auto& getSignal(SignalKey key) const
            {
                flip::SignalBase& signal_base = *m_signals.at(key);
                return dynamic_cast<flip::Signal<Args...>&>(signal_base);
            }
            
            //! @brief Checks if the object has this flag set.
            bool hasFlag(Flag flag) const;
            
        protected:
            
            //! @brief Adds a signal having singal key.
            template <class... Args>
            void addSignal(SignalKey key, model::Object& object)
            {
                m_signals.emplace(key, std::make_unique<flip::Signal<Args...>>(key, object));
            }
            
            //! @brief Sets one of the flag for the object.
            //! @details It is a modification of the model and requires commit.
            void setFlag(Flag flag);
            
            //! @brief Clear and replace all the object's inlets.
            void setInlets(flip::Array<Inlet> const& inlets);
            
            //! @brief Clear and replace all the object's outlets.
            void setOutlets(flip::Array<Outlet> const& outlets);
            
            //! @brief Adds an inlet at end of current inlet list.
            void pushInlet(std::set<PinType> type);
            
            //! @brief Adds an outlet at end of current outlet list.
            void pushOutlet(PinType type);
                
            //! @brief Sets the ratio height/width.
            //! @details If width was previously set. Height will adapt to ratio.
            void setRatio(double ratio);
                
            //! @brief Sets the minimal width that the object can have.
            //! @details Will recompute height and width if needed.
            void setMinWidth(double min_width);
                
            //! @brief Sets the minimal height that the object can have.
            //! @details Will recompute height and width if needed.
            void setMinHeight(double min_height);
                
        public: // internal methods
            
            //! @internal flip Default constructor
            Object(flip::Default&);
            
            //! @internal flip static declare method
            static void declare();
            
        private: // members
            
            std::map<SignalKey, std::unique_ptr<flip::SignalBase>> m_signals;
            
            flip::String            m_name;
            flip::String            m_text;
            flip::Array<Inlet>      m_inlets;
            flip::Array<Outlet>     m_outlets;
            flip::Collection<Flag>  m_flags;
            
            flip::Float         m_position_x;
            flip::Float         m_position_y;
            flip::Float         m_width;
            flip::Float         m_height;
            flip::Float         m_min_width;
            flip::Float         m_min_height;
            flip::Float         m_ratio;
                
            
            friend class Factory;
        
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(model::Object&&) = delete;
            model::Object& operator=(model::Object const&) = delete;
            model::Object& operator=(model::Object&&) = delete;
        };
    }
}
