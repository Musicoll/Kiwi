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

#ifndef KIWI_ENGINE_BEACON_HPP_INCLUDED
#define KIWI_ENGINE_BEACON_HPP_INCLUDED

#include "KiwiEngineDef.hpp"

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
        //! @see Beacon::Factory
        //! @see Beacon::Castaway
        class Beacon
        {
        public: // methods
            
            class Castaway;
            
            //! @brief Gets the name of the beacon.
            inline std::string getName() const {return m_name;}
            
            //! @brief Destructor.
            ~Beacon() = default;
            
            //! @brief Adds a castaway in the binding list of the beacon.
            void bind(Castaway& castaway);
            
            //! @brief Removes a castaways from the binding list of the beacon.
            void unbind(Castaway& castaway);
            
            //! @brief Dispatch message to beacon castaways.
            void dispatch(std::vector<Atom> const& args);
            
        public: // nested classes
            
            // ================================================================================ //
            //                                  BEACON CASTAWAY                                 //
            // ================================================================================ //
            
            //! @brief The beacon castaway can be binded to a beacon.
            class Castaway
            {
            public:
                virtual ~Castaway() {}
                virtual void receive(std::vector<Atom> const& args) = 0;
            };
            
            // ================================================================================ //
            //                                  BEACON FACTORY                                  //
            // ================================================================================ //
            
            //! @brief The beacon factory is used to create beacons.
            class Factory
            {
            public:
                Factory() = default;
                ~Factory() = default;
                Beacon& getBeacon(std::string const& name);
                
            private:
                std::map<std::string, std::unique_ptr<Beacon>> m_beacons;
            };
            
        private: // methods
            
            //! @internal Constructor.
            Beacon(std::string const& name);
            friend class Factory;
            
        private: // members
            
            const std::string   m_name;
            std::set<Castaway*> m_castaways;
            
        private: // deleted methods
            
            Beacon(Beacon const&) = delete;
            Beacon(Beacon&&) = delete;
            Beacon& operator=(Beacon const&) = delete;
            Beacon& operator=(Beacon&&) = delete;
        };
    };
};


#endif // KIWI_ENGINE_BEACON_HPP_INCLUDED
