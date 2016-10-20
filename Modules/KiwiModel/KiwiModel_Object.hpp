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

#ifndef KIWI_MODEL_OBJECT_HPP_INCLUDED
#define KIWI_MODEL_OBJECT_HPP_INCLUDED

#include <KiwiModel/KiwiModel_Atom.hpp>
#include <mutex>
#include <algorithm>
#include <exception>
#include <set>

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
            
            //! @brief Returns the number of inlets having same type.
            size_t getNumberOfInlets(PinType type) const;
            
            //! @brief Returns the inlet index among inlets having same type.
            size_t getInletIndex(size_t index, PinType type) const;
            
            //! @brief Returns true if the inlets changed.
            bool inletsChanged() const noexcept;
            
            //! @brief Returns the number of outlets.
            flip::Array<Outlet> const& getOutlets() const;
            
            //! @brief Returns the outlets at corresponding index.
            Outlet const& getOutlet(size_t index) const;
            
            //! @brief Returns the number of outlets.
            size_t getNumberOfOutlets() const;
            
            //! @brief Returns the number of inlets having same type.
            size_t getNumberOfOutlets(PinType type) const;
            
            //! @brief Returns the outlet index among outlets of same type.
            size_t getOutletIndex(size_t index, PinType type) const;
            
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
            
        protected:
            
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
            
            flip::String        m_name;
            flip::String        m_text;
            flip::Array<Inlet>  m_inlets;
            flip::Array<Outlet> m_outlets;
            
            flip::Float         m_position_x;
            flip::Float         m_position_y;
            flip::Float         m_width;
            flip::Float         m_height;
            
            friend class Factory;
        
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(model::Object&&) = delete;
            model::Object& operator=(model::Object const&) = delete;
            model::Object& operator=(model::Object&&) = delete;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
