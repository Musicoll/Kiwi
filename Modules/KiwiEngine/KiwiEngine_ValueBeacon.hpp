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

#ifndef KIWI_ENGINE_VALUE_BEACON_HPP_INCLUDED
#define KIWI_ENGINE_VALUE_BEACON_HPP_INCLUDED

#include "KiwiEngine_Listeners.hpp"

#include <string>
#include <map>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        //! @brief The beacon is unique and matchs to a "unique" string in the scope of a beacon factory and can be used to bind beacon's castaways.
        //! @details The beacon are uniques in the scope of a beacon factory and matchs to a string.
        //! If you create a beacon with a string that already matchs to a beacon of the beacon factory,
        //! it will return this beacon otherwise it will create a new beacon.
        //! Thus, the beacons can be used to bind, unbind and retrieve castways.
        //! After recovering a castaway, you should dynamically cast it to the class you expect.
        //! More often, this will be a kiwi Object.
        //! @see ValueBeacon::Factory
        //! @see ValueBeacon::Castaway
        class ValueBeacon
        {
        public: // methods
            
            class Value;
            class Castaway;
            class Factory;
            
            //! @brief Gets the name of the beacon.
            inline std::string getName() const {return m_name;}
            
            //! @brief Destructor.
            ~ValueBeacon() = default;
            
            //! @brief Adds a castaway in the binding list of the beacon.
            void bind(Castaway& castaway);
            
            //! @brief Removes a castaways from the binding list of the beacon.
            void unbind(Castaway& castaway);
            
            //! @brief Returns true if this beacon is currently owning a value object.
            bool hasValue();
            
            //! @brief Returns the owned Value.
            Value* getValue();
            
            //! @brief Set a new Beacon Value.
            void setValue(std::unique_ptr<Value> new_value);
            
        private: // methods
            
            //! @internal Constructor.
            ValueBeacon(std::string const& name);
            friend class ValueBeacon::Factory;
            
        private: // members
            
            const std::string       m_name;
            Listeners<Castaway>     m_castaways;
            std::unique_ptr<Value>  m_value;
            
        private: // deleted methods
            
            ValueBeacon(ValueBeacon const&) = delete;
            ValueBeacon(ValueBeacon&&) = delete;
            ValueBeacon& operator=(ValueBeacon const&) = delete;
            ValueBeacon& operator=(ValueBeacon&&) = delete;
        };
        
        // ================================================================================ //
        //                                   BEACON VALUE                                   //
        // ================================================================================ //
        
        //! @brief The beacon castaway can be binded to a beacon.
        class ValueBeacon::Value
        {
        public:
            virtual ~Value() {}
        };
        
        // ================================================================================ //
        //                                     ObjectValue                                  //
        // ================================================================================ //
        
        template<class TObjectClass>
        class ObjectValue : public ValueBeacon::Value
        {
        public:
            
            ObjectValue(TObjectClass* object) : m_object(object) {};
            
            virtual ~ObjectValue() = default;
            
            operator bool()
            {
                return (m_object != nullptr);
            }
            
            TObjectClass* get()
            {
                return m_object;
            }
            
        private:
            
            TObjectClass* m_object;
        };
        
        // ================================================================================ //
        //                                  BEACON CASTAWAY                                 //
        // ================================================================================ //
        
        //! @brief The beacon castaway can be binded to a beacon.
        class ValueBeacon::Castaway
        {
        public:
            virtual ~Castaway() = default;
            
            //! @brief Called when the value has changed.
            virtual void valueChanged(ValueBeacon& beacon) = 0;
        };
        
        // ================================================================================ //
        //                                  BEACON FACTORY                                  //
        // ================================================================================ //
        
        //! @brief The beacon factory is used to create beacons.
        class ValueBeacon::Factory
        {
        public:
            Factory() = default;
            ~Factory() = default;
            ValueBeacon& useValueBeacon(std::string const& name);
            
        private:
            std::map<std::string, std::unique_ptr<ValueBeacon>> m_beacons;
        };
    };
};


#endif // KIWI_ENGINE_VALUE_BEACON_HPP_INCLUDED
