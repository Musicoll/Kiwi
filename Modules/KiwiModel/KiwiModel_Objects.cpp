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

#include "KiwiModel_Objects.hpp"

#include "KiwiModel_DataModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        void NewBox::declare()
        {
            if(DataModel::has<NewBox>()) return;
            
            DataModel::declare<NewBox>()
            .name("cicm.kiwi.object.NewBox")
            .inherit<model::Object>();
            
            Factory::add<NewBox>("newbox");
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
            if(DataModel::has<ErrorBox>()) return;
            
            DataModel::declare<ErrorBox>()
            .name("cicm.kiwi.object.ErrorBox")
            .inherit<model::Object>();
            
            Factory::add<ErrorBox>("errorbox");
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
            if(DataModel::has<Plus>()) return;
            
            DataModel::declare<Plus>()
            .name("cicm.kiwi.object.Plus")
            .inherit<model::Object>();
            
            Factory::add<Plus>("plus", {"+"});
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
            if(DataModel::has<Times>()) return;
            
            DataModel::declare<Times>()
            .name("cicm.kiwi.object.Times")
            .inherit<model::Object>();
            
            Factory::add<Times>("times", {"*"});
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
            if(DataModel::has<Print>()) return;
            
            DataModel::declare<Print>()
            .name("cicm.kiwi.object.Print")
            .inherit<model::Object>();
            
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
            if(DataModel::has<Receive>()) return;
            
            DataModel::declare<Receive>()
            .name("cicm.kiwi.object.Receive")
            .inherit<model::Object>();
            
            Factory::add<Receive>("receive", {"r"});
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
            if(DataModel::has<Loadmess>()) return;
            
            DataModel::declare<Loadmess>()
            .name("cicm.kiwi.object.Loadmess")
            .inherit<model::Object>();
            
            Factory::add<Loadmess>("loadmess");
        }
        
        // ================================================================================ //
        //                                  OBJECT DAC~                                     //
        // ================================================================================ //
        
        DacTilde::DacTilde(std::string const& name, std::vector<Atom> const& args)
        {
            size_t channels = 0;
            
            for(const Atom& atom : args)
            {
                if(atom.isNumber())
                {
                    channels++;
                }
            }
            
            if(channels == 0) channels = 2;
            
            pushInlet({PinType::IType::Signal, PinType::IType::Control});
            
            for (int i = 1; i < channels; ++i)
            {
                pushInlet({PinType::IType::Signal});
            }
        }
        
        void DacTilde::declare()
        {
            if(DataModel::has<DacTilde>()) return;
            
            DataModel::declare<DacTilde>()
            .name("cicm.kiwi.object.DacTilde")
            .inherit<model::Object>();
            
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
            if(DataModel::has<OscTilde>()) return;
            
            DataModel::declare<OscTilde>()
            .name("cicm.kiwi.object.OscTilde")
            .inherit<model::Object>();

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
            if(DataModel::has<TimesTilde>()) return;
            
            DataModel::declare<TimesTilde>()
            .name("cicm.kiwi.object.TimesTilde")
            .inherit<model::Object>();
            
            Factory::add<TimesTilde>("times~", {"*~"});
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
            if(DataModel::has<PlusTilde>()) return;
            
            DataModel::declare<PlusTilde>()
            .name("cicm.kiwi.object.PlusTilde")
            .inherit<model::Object>();
            
            Factory::add<PlusTilde>("plus~", {"+~"});
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
            if(DataModel::has<SigTilde>()) return;
            
            DataModel::declare<SigTilde>()
            .name("cicm.kiwi.object.SigTilde")
            .inherit<model::Object>();
            
            Factory::add<SigTilde>("sig~");
        }
    }
}
