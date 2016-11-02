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

#ifndef KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED
#define KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED

#include "KiwiModel_Object.hpp"
#include "KiwiModel_Factory.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        class NewBox : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            NewBox(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            NewBox(std::string const& name, std::vector<Atom> const& args);

            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        class ErrorBox : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ErrorBox(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ErrorBox(std::string const& name, std::vector<Atom> const& args);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setInlets(flip::Array<Inlet> const& inlets);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setOutlets(flip::Array<Outlet> const& outlets);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPlus : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectPlus(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectPlus(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        class ObjectTimes : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectTimes(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectTimes(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        class ObjectPrint : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectPrint(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectPrint(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class ObjectReceive : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectReceive(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectReceive(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT OSC~                                   //
        // ================================================================================ //
        
        class OscTilde : public model::Object
        {
        public:
            //! @brief flip Default Constructor
            OscTilde(flip::Default& d): model::Object(d){};

            //! @brief Constructor
            OscTilde(std::string const& name, std::vector<Atom> const& args);

            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                     OBJECT DAC~                                  //
        // ================================================================================ //
        
        
        class DacTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            DacTilde(flip::Default& d): model::Object(d){}
            
            //! @brief Constructor
            DacTilde(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT *~                                   //
        // ================================================================================ //
        
        class TimesTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            TimesTilde(flip::Default& d): model::Object(d){};
            
            //! @brief Constructor
            TimesTilde(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      OBJECT +~                                   //
        // ================================================================================ //
        
        class PlusTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            PlusTilde(flip::Default& d): model::Object(d){};
            
            //! @brief Constructor
            PlusTilde(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      OBJECT SIG~                                 //
        // ================================================================================ //
        
        class SigTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            SigTilde(flip::Default& d): model::Object(d){};
            
            //! @brief Constructor
            SigTilde(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
    }
}


#endif // KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED
