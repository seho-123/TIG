<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=3.0.90.23 SP05?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1" />
  <TaskClass Name="Cyclic#2">
    <Task Name="MotorDrive" Source="MotorDriver.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="PowerSuppl" Source="PowerSupplyDriver.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#3">
    <Task Name="Master" Source="Master.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Menu" Source="Menu.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#4" />
  <DataObjects />
  <NcDataObjects />
  <VcDataObjects />
  <Binaries>
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="gcloader" Source="" Memory="SystemROM" Language="Binary" />
  </Binaries>
  <Libraries>
    <LibraryObject Name="dvframe" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="runtime" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="loopconr" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brsystem" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="burtrap" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="standard" Source="" Memory="SystemROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsBrMath" Source="Libraries.AsBrMath.lby" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>