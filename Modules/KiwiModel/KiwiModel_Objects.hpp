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
        
        class Plus : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            Plus(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            Plus(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        class Times : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            Times(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            Times(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        class Print : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            Print(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            Print(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class Receive : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            Receive(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            Receive(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                  OBJECT LOADMESS                                 //
        // ================================================================================ //
        
        class Loadmess : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            Loadmess(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            Loadmess(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      OBJECT DELAY                                //
        // ================================================================================ //
        
        class Delay : public model::Object
        {
        public: // methods
            
            //! @brief flip Default Constructor
            Delay(flip::Default& d) : model::Object(d) {};
            
            //! @brief Constructor
            Delay(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      OBJECT PIPE                                 //
        // ================================================================================ //
        
        class Pipe : public model::Object
        {
        public: // methods
            
            //! @brief flip Default Constructor
            Pipe(flip::Default& d) : model::Object(d) {};
            
            //! @brief Constructor
            Pipe(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                   OBJECT METRO                                   //
        // ================================================================================ //
        
        class Metro : public model::Object
        {
        public: // methods
            
            //! @brief flip Default Constructor
            Metro(flip::Default& d) : model::Object(d) {};
            
            //! @brief Constructor
            Metro(std::string const& name, std::vector<Atom> const& args);
            
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
        //                                  OBJECT ADC~                                     //
        // ================================================================================ //
        
        class AdcTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            AdcTilde(flip::Default& d): model::Object(d){}
            
            //! @brief Constructor
            AdcTilde(std::string const& name, std::vector<Atom> const& args);
            
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
        
        // ================================================================================ //
        //                                      OBJECT DELAYTILDE~                          //
        // ================================================================================ //
        
        class DelayTilde : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            DelayTilde(flip::Default& d): model::Object(d){};
            
            //! @brief Constructor
            DelayTilde(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
    }
}
