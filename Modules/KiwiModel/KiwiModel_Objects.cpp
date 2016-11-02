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
            .name("cicm.kiwi.NewBox")
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
            .name("cicm.kiwi.ErrorBox")
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
 
        void ObjectPlus::declare()
        {
            if(DataModel::has<ObjectPlus>()) return;
            
            DataModel::declare<ObjectPlus>()
            .name("cicm.kiwi.ObjectPlus")
            .inherit<model::Object>();
            
            Factory::add<ObjectPlus>("plus", {"+"});
        }
        
        ObjectPlus::ObjectPlus(std::string const& name, std::vector<Atom> const& args)
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
        
        void ObjectTimes::declare()
        {
            if(DataModel::has<ObjectTimes>()) return;
            
            DataModel::declare<ObjectTimes>()
            .name("cicm.kiwi.ObjectTimes")
            .inherit<model::Object>();
            
            Factory::add<ObjectTimes>("times", {"*"});
        }
        
        ObjectTimes::ObjectTimes(std::string const& name, std::vector<Atom> const& args)
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
        
        ObjectPrint::ObjectPrint(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control});
        }
        
        void ObjectPrint::declare()
        {
            if(DataModel::has<ObjectPrint>()) return;
            
            DataModel::declare<ObjectPrint>()
            .name("cicm.kiwi.ObjectPrint")
            .inherit<model::Object>();
            
            Factory::add<ObjectPrint>("print");
        }
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        ObjectReceive::ObjectReceive(std::string const& name, std::vector<Atom> const& args)
        {
            pushOutlet(PinType::IType::Control);
        }
        
        void ObjectReceive::declare()
        {
            if(DataModel::has<ObjectReceive>()) return;
            
            DataModel::declare<ObjectReceive>()
            .name("cicm.kiwi.ObjectReceive")
            .inherit<model::Object>();
            
            Factory::add<ObjectReceive>("receive", {"r"});
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
            .name("cicm.kiwi.DacTilde")
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
            .name("cicm.kiwi.OscTilde")
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
            .name("cicm.kiwi.TimesTilde")
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
            .name("cicm.kiwi.PlusTilde")
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
            .name("cicm.kiwi.SigTilde")
            .inherit<model::Object>();
            
            Factory::add<SigTilde>("sig~");
        }
    }
}
