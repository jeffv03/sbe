# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/simics-debug-framework.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2018
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
import os
import os.path
import subprocess
import re
import random
import sys
import imp
import struct

testIstepAuto = imp.load_source("testIstepAuto", os.environ['SBE_TOOLS_PATH'] + "/testIstepAuto.py")
sbeDebug = imp.load_source("sbeDebug", os.environ['SBE_TOOLS_PATH'] + "/sbe-debug.py")
err = False

syms = {};

def check_sbe_tools_path ():
  global SBE_TOOLS_PATH
  SBE_TOOLS_PATH = os.environ['SBE_TOOLS_PATH'];

def get_dd_level(procNr = 0):
    cmd = "pipe \"p9Proc"+str(procNr)+".sbe.mibo_space.map\" \"cat > temp.map\""
    ( rc, out )  =   quiet_run_command( cmd, output_modes.regular )
    if ( rc ):
        print "simics ERROR running %s: %d "%( cmd, rc )
    ddlevel = 'DD1'
    with open('temp.map', 'r') as f:
        map = f.read()
        map = map.split()
        if map[map.index('p9Proc'+str(procNr)+'.sbe.fi2c_bo:fi2cfsm')-1] == '0x80000000':
            ddlevel = "DD1"
        if map[map.index('p9Proc'+str(procNr)+'.sbe.fi2c_bo:fi2cfsm')-1] == '0xff800000':
            ddlevel = "DD2"
    print "running image - ["+ddlevel+"]"
    return ddlevel

def register_sbe_debug_framework_tools():
    check_sbe_tools_path ()
    fillSymTable()
    # Create command hook.
    new_command("sbe-istep",testIstepAuto.sbe_istep_func,
                 args = [arg(float_t, "Major/start istep"), arg(float_t, "Minor/end istep")],
                 alias = "istep",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for istep ",
                 doc = "")
    new_command("sbe-trace", collectTrace,
                 args = [arg(int_t, "procNr")],
                 alias = "strace",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for trace ",
                 doc = "")
    new_command("sbe-stack", collectStackUsage,
                 args = [arg(int_t, "procNr")],
                 alias = "sstack",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for stack usage ",
                 doc = "")
    new_command("sbe-ddlevel", get_dd_level,
                 args = [arg(int_t, "procNr")],
                 alias = "sddlevel",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for dd level ",
                 doc = "")
    new_command("sbe-attrdump", collectAttr,
                 args = [arg(int_t, "procNr")],
                 alias = "sattrdump",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for runtime attribute dump ",
                 doc = "")
    new_command("sbe-regffdc", collectRegFfdc,
                 args = [arg(int_t, "procNr")],
                 alias = "sregffdc",
                 type = ["sbe-commands"],
                 short = "Runs the debug framework for register ffdc ",
                 doc = "")
    print "SBE Debug Framework: Registered tool:", "sbe-istep"
    print "SBE Debug Framework: Registered tool:", "sbe-trace"
    print "SBE Debug Framework: Registered tool:", "sbe-stack"
    print "SBE Debug Framework: Registered tool:", "sbe-ddlevel"
    print "SBE Debug Framework: Registered tool:", "sbe-attrdump"
    print "SBE Debug Framework: Registered tool:", "sbe-regffdc"


def fillSymTable():
    symFile = SBE_TOOLS_PATH + "/sbe_"+get_dd_level()+".syms"
    f = open( symFile, 'r')
    for line in f:
        words = line.split()
        if( len( words ) == 4 ):
            syms[words[3]] = [words[0], words[1]]

# Print least available stack of each thread in SBE during a Run.
#
# Logic is - during init, ppe kernel fills the stack memory with '0xEFCDAB03'.
# So while traversing the stack, starting from lowest memory to the top,
# the first memory address where the pattern('0xEFCDAB03') is broken,
# will be the deepest stack usage point of tht thread during the run
def collectStackUsage ( procNr ):
  threads = ('sbeSyncCommandProcessor_stack',
             'sbeCommandReceiver_stack',
             'sbe_Kernel_NCInt_stack',
             'sbeAsyncCommandProcessor_stack')
  print "==================================Stack usage==================================="
  # Dump stack memory to binary files
  for thread in threads:
    cmd = "pipe \"p9Proc" + `procNr` + ".sbe.mibo_space.x 0x" + syms[thread][0] + " 0x"+syms[thread][1]+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> "+thread+"\""
    ( rc, out )  =   quiet_run_command( cmd, output_modes.regular )
    if ( rc ):
        print "simics ERROR running %s: %d "%( cmd, rc )

  print "Thread".ljust(40)+"Least Available[bytes]".ljust(30)+"Max usage[%]"
  for thread in threads:
    with open(thread, "rb") as f:
        word = struct.unpack('I', f.read(4))[0]
        leastAvailable = 0
        while (1):
            if (word == int("0xEFCDAB03", 16)):
                leastAvailable += 4
                word = struct.unpack('I', f.read(4))[0]
            else:
                break
        print str("["+thread+"]").ljust(40) + str(leastAvailable).ljust(30) + str("%.2f" % (100 * (1 - (leastAvailable/float(int("0x"+syms[thread][1], 16))))))

def collectAttr( procNr ):
  cmd1= "pipe \"p9Proc" + `procNr` + ".sbe.mibo_space.x " + '0xFFFE8000' + " "+hex(96*1024)+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> DumpFullPIBMEM\""
  ( rc, out )  =   quiet_run_command( cmd1, output_modes.regular )
  if ( rc ):
    print "simics ERROR running %s: %d "%( cmd1, rc )
  ddlevel = get_dd_level(procNr)
  sbeDebug.ddsuffix = ddlevel
  sbeDebug.target = 'FILE'
  sbeDebug.file_path = 'DumpFullPIBMEM'
  sbeDebug.fillSymTable()
  sbeDebug.collectAttr()

def collectRegFfdc( procNr ):
  cmd1= "pipe \"p9Proc" + `procNr` + ".sbe.mibo_space.x " + '0xFFFE8000' + " "+hex(96*1024)+"\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> DumpFullPIBMEM\""
  ( rc, out )  =   quiet_run_command( cmd1, output_modes.regular )
  if ( rc ):
    print "simics ERROR running %s: %d "%( cmd1, rc )
  ddlevel = get_dd_level(procNr)
  sbeDebug.ddsuffix = ddlevel
  sbeDebug.target = 'FILE'
  sbeDebug.file_path = 'DumpFullPIBMEM'
  sbeDebug.fillSymTable()
  sbeDebug.ppeStateFfdc()

def collectTrace ( procNr ):
  fileName = "sbe_" + `procNr` + "_tracMERG"
  cmd1 = "pipe \"p9Proc" + `procNr` + ".sbe.mibo_space.x 0x" + syms['g_pk_trace_buf'][0] + " 0x2028\" \"sed 's/^p:0x........ //g' | sed 's/ ................$//g' | sed 's/ //g' | xxd -r -p> ppetrace.bin\""
  cmd2 = "shell \"" + SBE_TOOLS_PATH + "/ppe2fsp ppetrace.bin sbetrace.bin \""
  cmd3 = "shell \"" + "fsp-trace -s " + SBE_TOOLS_PATH + "/sbeStringFile_"+get_dd_level(procNr)+" sbetrace.bin >" +  fileName + "\""
  cmd4 = "shell \"" + "cat " + fileName + "\""

  ( rc, out )  =   quiet_run_command( cmd1, output_modes.regular )
  if ( rc ):
    print "simics ERROR running %s: %d "%( cmd1, rc )

  run_command ( cmd2 )
  run_command ( cmd3 )
  run_command ( cmd4 )

# MAGIC_INSTRUCTION hap handler
# arg contains the integer parameter n passed to MAGIC_INSTRUCTION(n)
# See src/include/arch/ppc.H for the definitions of the magic args.
# SBE  magic args should range 8000..8190.
def sbe_magic_instruction_callback(user_arg, cpu, inst_num):
    # Check if its for ppe processor
    if(SIM_get_class_name(SIM_object_class(cpu)) != "ppe"):
        return;

    if inst_num == 8000: #MAGIC_SIMICS_CHECK
        iface = SIM_get_interface(cpu, "int_register")
        iface.write(iface.get_number("r3"), 1)
        print "SBE::isSimicsRunning = true"

# Run the registration automatically whenever this script is loaded.
register_sbe_debug_framework_tools()

# Register the magic instruction hap handler (a callback).
# Currently registering a range does not work on simics for sbe
SIM_hap_add_callback( "Core_Magic_Instruction", sbe_magic_instruction_callback, None )

