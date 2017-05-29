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
        
        std::string NewBox::getIODescription(bool is_inlet, size_t index) const
        {
            return "(nothing here)";
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
        
        std::string ErrorBox::getIODescription(bool is_inlet, size_t index) const
        {
            return "(nothing here)";
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
        
        std::string Plus::getIODescription(bool is_inlet, size_t index) const
        {
            return is_inlet ? (index == 0) ? "Left operand" : "Right operand" : "Result";
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
        
        std::string Times::getIODescription(bool is_inlet, size_t index) const
        {
            return is_inlet ? (index == 0) ? "Left operand" : "Right operand" : "Result";
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
        
        std::string Print::getIODescription(bool is_inlet, size_t index) const
        {
            return "Message to post in the Kiwi console";
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
        
        std::string Receive::getIODescription(bool is_inlet, size_t index) const
        {
            if(!is_inlet)
            {
                return "Receive messages";
            }
            
            return {};
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
        
        std::string Loadmess::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                return "Receive messages when the patch is loaded";
            }
            
            return {};
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
        
        std::string Delay::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "bang gets delayed, message \"stop\" cancels";
                }
                else if(index == 1)
                {
                    return "Set delay time";
                }
            }
            else
            {
                return "Delayed bang";
            }
            
            return {};
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
        
        std::string Pipe::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "Anything to be delayed";
                }
                else if(index == 1)
                {
                    return "Set delay time";
                }
            }
            else
            {
                return "Delayed messages";
            }
            
            return {};
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
        
        std::string Metro::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "Start/Stop metronome";
                }
                else if(index == 1)
                {
                    return "Set time interval";
                }
            }
            else
            {
                return "Outputs metronome ticks as bang";
            }
            
            return {};
        }
        
        // ================================================================================ //
        //                                  OBJECT ADC~                                     //
        // ================================================================================ //
        
        std::vector<size_t> parseArgsAsChannelRoutes(std::vector<Atom> const& args)
        {
            std::vector<size_t> routes;
            
            for(Atom const& arg : args)
            {
                if (arg.isNumber())
                {
                    if (arg.getInt() <= 0)
                    {
                        throw std::runtime_error("null or negative channel");
                    }
  
                    routes.push_back(arg.getInt() - 1);
                }
                else if(arg.isString())
                {
                    std::string inputs(arg.getString());
                    
                    size_t sep_pos = inputs.find(":");
                    
                    if (sep_pos == std::string::npos)
                    {
                        throw std::runtime_error("wrong symbol syntax");
                    }
                    
                    int left_input = std::stoi(inputs.substr(0, sep_pos)) - 1;
                    int right_input = std::stoi(inputs.substr(inputs.find(":") + 1)) - 1;
                    
                    if (left_input < 0 || right_input < 0)
                    {
                        throw std::runtime_error("null or negative channel");
                    }
                    
                    const bool rev = left_input > right_input;
                    
                    for (int channel = left_input;
                         rev ? channel >= right_input : channel <= right_input;
                         rev ? --channel : ++channel)
                    {
                        routes.push_back(channel);
                    }
                }
            }
            
            if (routes.empty())
            {
                routes = {0, 1};
            }
            
            return routes;
        }
        
        AdcTilde::AdcTilde(std::string const& name, std::vector<Atom> const& args)
        {
            size_t channels = parseArgsAsChannelRoutes(args).size();
            
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
        
        std::string AdcTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                return "Start/Stop dsp";
            }
            else
            {
                if(index < getNumberOfOutlets())
                {
                    auto text_atoms = AtomHelper::parse(getText());
                    text_atoms.erase(text_atoms.cbegin());
                    const auto routes = parseArgsAsChannelRoutes(text_atoms);
                    
                    return "(signal) Audio In Channel " + std::to_string(routes[index] + 1);
                }
            }
            
            return {};
        }
        
        // ================================================================================ //
        //                                  OBJECT DAC~                                     //
        // ================================================================================ //
        
        DacTilde::DacTilde(std::string const& name, std::vector<Atom> const& args)
        {
            size_t channels = parseArgsAsChannelRoutes(args).size();
            
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

        std::string DacTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index < getNumberOfInlets())
                {
                    auto text_atoms = AtomHelper::parse(getText());
                    text_atoms.erase(text_atoms.cbegin());
                    const auto routes = parseArgsAsChannelRoutes(text_atoms);
                    
                    if(index == 0)
                    {
                        return "Start/Stop dsp, (signal) Audio Out Channel " + std::to_string(routes[0] + 1);
                    }
                    else
                    {
                        return "(signal) Audio Out Channel " + std::to_string(routes[index] + 1);
                    }
                }
            }
            
            return {};
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
        
        std::string OscTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "(signal/float) Set frequency";
                }
                else if(index == 1)
                {
                    return "(signal/float) Set phase (0-1)";
                }
            }
            else
            {
                return "(signal) Output";
            }
            
            return {};
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
        
        std::string TimesTilde::getIODescription(bool is_inlet, size_t index) const
        {
            return is_inlet ? (index == 0) ? "(signal) Left operand" : "(signal) Right operand" : "(signal) Output";
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
        
        std::string PlusTilde::getIODescription(bool is_inlet, size_t index) const
        {
            return is_inlet ? (index == 0) ? "(signal) Left operand" : "(signal) Right operand" : "(signal) Output";
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
        
        std::string SigTilde::getIODescription(bool is_inlet, size_t index) const
        {
            return is_inlet ? "(float/int) Set signal value" : "(signal) Output value";
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
        
        std::string DelayTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "(signal) Input to be delayed";
                }
                else if(index == 1)
                {
                    return "(signal/float) Delay time (ms)";
                }
                else if(index == 2)
                {
                    return "(float) Feedback (0-1)";
                }
            }
            else
            {
                return "(signal) Delayed output signal";
            }
            
            return {};
        }
    }
}
