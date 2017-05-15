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

#include "KiwiModel_Objects.h"

#include "KiwiModel_DataModel.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        void NewBox::declare()
        {
            Factory::add<NewBox>("newbox").setInternal(true);
        }
        
        NewBox::NewBox(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control});
        }
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        void ErrorBox::declare()
        {
            Factory::add<ErrorBox>("errorbox").setInternal(true);
        }
        
        ErrorBox::ErrorBox(std::string const& name, std::vector<Atom> const& args)
        {
        }
        
        void ErrorBox::setInlets(flip::Array<Inlet> const& inlets)
        {
            model::Object::setInlets(inlets);
        }
        
        void ErrorBox::setOutlets(flip::Array<Outlet> const& outlets)
        {
            model::Object::setOutlets(outlets);
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
 
        void Plus::declare()
        {
            Factory::add<Plus>("plus").addAlias("+");
        }
        
        Plus::Plus(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control});
            
            if (args.empty() || !args[0].isNumber())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        // ================================================================================ //
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        void Times::declare()
        {
            Factory::add<Times>("times").addAlias("*");
        }
        
        Times::Times(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control});
            
            if (args.empty() || !args[0].isNumber())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        Print::Print(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control});
        }
        
        void Print::declare()
        {
            Factory::add<Print>("print");
        }
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        Receive::Receive(std::string const& name, std::vector<Atom> const& args)
        {
            pushOutlet(PinType::IType::Control);
        }
        
        void Receive::declare()
        {
            Factory::add<Receive>("receive").addAlias("r");
        }
        
        // ================================================================================ //
        //                                  OBJECT LOADMESS                                 //
        // ================================================================================ //
        
        Loadmess::Loadmess(std::string const& name, std::vector<Atom> const& args)
        {
            pushOutlet(PinType::IType::Control);
        }
        
        void Loadmess::declare()
        {
            Factory::add<Loadmess>("loadmess");
        }
        
        // ================================================================================ //
        //                                  OBJECT DELAY                                    //
        // ================================================================================ //
        
        Delay::Delay(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() > 1 || (args.size() == 1 && !args[0].isNumber()))
            {
                throw std::runtime_error("wrong argument for object Delay");
            }
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        void Delay::declare()
        {
            Factory::add<Delay>("delay");
        }
        
        // ================================================================================ //
        //                                      OBJECT PIPE                                 //
        // ================================================================================ //
        
        Pipe::Pipe(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() > 1 || (args.size() == 1 && !args[0].isNumber()))
            {
                throw std::runtime_error("wrong argument for object Pipe");
            }
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        void Pipe::declare()
        {
            Factory::add<Pipe>("pipe");
        }
        
        // ================================================================================ //
        //                                   OBJECT METRO                                   //
        // ================================================================================ //
        
        Metro::Metro(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() < 1 || (args.size() == 1 && !args[0].isNumber()))
            {
                throw std::runtime_error("wrong argument for object Metro");
            }
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        void Metro::declare()
        {
            Factory::add<Metro>("metro");
        }
        
        // ================================================================================ //
        //                                  OBJECT ADC~                                     //
        // ================================================================================ //
        
        size_t getNumberOfChannels(std::vector<Atom> const& args)
        {
            size_t channels = 0;
            
            for(const Atom& atom : args)
            {
                if(atom.isNumber())
                {
                    if (atom.getInt() <= 0)
                    {
                        throw std::runtime_error("null or negative channel");
                    }
                    
                    channels++;
                }
                else if(atom.isString())
                {
                    std::string inputs(atom.getString());
                    
                    size_t sep_pos = inputs.find(":");
                    
                    if (sep_pos == std::string::npos)
                    {
                        throw std::runtime_error("wrong symbol syntax");
                    }
                    
                    int left_input = std::stoi(inputs.substr(0, sep_pos));
                    int right_input = std::stoi(inputs.substr(inputs.find(":") + 1));
                    
                    if (left_input <= 0 || right_input <= 0)
                    {
                        throw std::runtime_error("null or negative channel");
                    }
                    
                    channels += std::abs(right_input - left_input) + 1;
                }
            }
            
            if(channels == 0) channels = 2;
            
            return channels;
        };
        
        AdcTilde::AdcTilde(std::string const& name, std::vector<Atom> const& args)
        {
            size_t channels = getNumberOfChannels(args);
            
            pushInlet({PinType::IType::Control});
            
            for (int i = 0; i < channels; ++i)
            {
                pushOutlet({PinType::IType::Signal});
            }
        }
        
        void AdcTilde::declare()
        {
            Factory::add<AdcTilde>("adc~");
        }
        
        // ================================================================================ //
        //                                  OBJECT DAC~                                     //
        // ================================================================================ //
        
        DacTilde::DacTilde(std::string const& name, std::vector<Atom> const& args)
        {
            size_t channels = getNumberOfChannels(args);
            
            pushInlet({PinType::IType::Signal, PinType::IType::Control});
            
            for (int i = 1; i < channels; ++i)
            {
                pushInlet({PinType::IType::Signal});
            }
        }
        
        void DacTilde::declare()
        {
            Factory::add<DacTilde>("dac~");
        }

        
        // ================================================================================ //
        //                                  OBJECT OSC~                                     //
        // ================================================================================ //
        
        OscTilde::OscTilde(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
         
            pushOutlet(PinType::IType::Signal);
        }
        
        void OscTilde::declare()
        {
            Factory::add<OscTilde>("osc~");
        }
        
        // ================================================================================ //
        //                                  OBJECT TIMES~                                   //
        // ================================================================================ //
        
        TimesTilde::TimesTilde(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Signal});
            
            if (args.empty() || !args[0].isNumber())
            {
                pushInlet({PinType::IType::Signal, PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void TimesTilde::declare()
        {
            Factory::add<TimesTilde>("times~").addAlias("*~");
        }
        
        // ================================================================================ //
        //                                  OBJECT PLUS~                                   //
        // ================================================================================ //
        
        PlusTilde::PlusTilde(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Signal});
            
            if (args.empty() || !args[0].isNumber())
            {
                pushInlet({PinType::IType::Signal, PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void PlusTilde::declare()
        {
            Factory::add<PlusTilde>("plus~").addAlias("+~");
        }
        
        // ================================================================================ //
        //                                  OBJECT SIG~                                     //
        // ================================================================================ //
        
        SigTilde::SigTilde(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.empty() || !args[0].isNumber())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void SigTilde::declare()
        {
            Factory::add<SigTilde>("sig~");
        }
        
        // ================================================================================ //
        //                                      OBJECT DELAYTILDE~                          //
        // ================================================================================ //
        
        DelayTilde::DelayTilde(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() > 0)
            {
                throw ("wrong argument for object delay~");
            }
            
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control});
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void DelayTilde::declare()
        {
            Factory::add<DelayTilde>("delaysimple~");
        }
    }
}
