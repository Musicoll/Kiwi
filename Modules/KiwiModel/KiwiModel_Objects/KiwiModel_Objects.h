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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_NewBox.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_ErrorBox.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Print.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Receive.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Slider.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Plus.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Times.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Delay.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Metro.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Pipe.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Bang.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Toggle.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_AdcTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_DacTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_OscTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Loadmess.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_SigTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_TimesTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_PlusTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_MeterTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_DelaySimpleTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_NoiseTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_PhasorTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_SahTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_SnapshotTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Trigger.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_LineTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Minus.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Divide.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Equal.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Less.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Greater.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Different.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Pow.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Modulo.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_MinusTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_DivideTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_LessTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_GreaterTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_EqualTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_DifferentTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_LessEqual.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_LessEqualTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_GreaterEqual.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_GreaterEqualTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Comment.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Pack.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Unpack.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Random.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Scale.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Select.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_NumberTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Hub.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Mtof.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Send.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Gate.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Switch.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_GateTilde.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_SwitchTilde.h>
