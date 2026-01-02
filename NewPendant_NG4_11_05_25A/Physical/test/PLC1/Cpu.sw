<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=3.0.90.23 SP05?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1" />
  <TaskClass Name="Cyclic#2" />
  <TaskClass Name="Cyclic#3">
    <Task Name="CK_test" Source="CK_test.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#4" />
  <DataObjects />
  <NcDataObjects />
  <VcDataObjects />
  <Binaries>
    <BinaryObject Name="gcloader" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="UserROM" Language="Binary" />
  </Binaries>
  <Libraries />
</SwConfiguration>