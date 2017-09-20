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

// ================================================================================ //
//                                  BASIC OBJECTS                                   //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_NewBox.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_ErrorBox.h>

// ================================================================================ //
//                                  DSP OBJECTS                                     //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_SigTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_PlusTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_TimesTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_OscTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_DelaySimpleTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_AdcTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_DacTilde.h>

// ================================================================================ //
//                                  MATH OBJECTS                                    //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Math/KiwiModel_Plus.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Math/KiwiModel_Times.h>

// ================================================================================ //
//                                  MESSAGE OBJECTS                                 //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message/KiwiModel_Print.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message/KiwiModel_Loadmess.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message/KiwiModel_Receive.h>

// ================================================================================ //
//                                  TIME OBJECTS                                    //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Time/KiwiModel_Metro.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Time/KiwiModel_Pipe.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Time/KiwiModel_Delay.h>

// ================================================================================ //
//                                 CONTROLLER OBJECTS                               //
// ================================================================================ //

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Bang.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Slider.h>

