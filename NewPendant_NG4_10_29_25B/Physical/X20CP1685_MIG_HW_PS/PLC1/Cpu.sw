<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio FileVersion="4.9"?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1">
    <Task Name="LincolnPS" Source="Universal.LincolnPS.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Telnet" Source="Universal.Telnet.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="TML_srv" Source="Universal.TML_srv.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="AVC_SYNCH" Source="FourAxisTigCtrl.AVC_SYNCH.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#2">
    <Task Name="TIG_PSprc" Source="Universal.TIG_PSprc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="WRST" Source="Universal.WRST.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="XMLHelper" Source="Universal.XMLHelper.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="BR_DC" Source="Universal.BR_DC.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="AVCprc" Source="Universal.AVCprc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="OSC_TMLprc" Source="Universal.OSC_TMLprc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="WIREprc" Source="Universal.WIREprc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="HW_PSprc" Source="Universal.HW_PSprc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="GasCoolPrc" Source="Universal.GasCoolPrc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="WireBlkPrc" Source="Universal.WireBlkPrc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="IndexPrc" Source="Universal.IndexPrc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="TvlRotPrc" Source="Universal.TvlRotPrc.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#3">
    <Task Name="MV" Source="Universal.MV.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="HMI_M" Source="FourAxisTigCtrl.HMI_M.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="MiscOper" Source="Universal.MiscOper.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="PermSaving" Source="FourAxisTigCtrl.PermSaving.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Main2" Source="FourAxisTigCtrl.Main2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="DC2DirMtr" Source="Universal.DC2DirMtr.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="TML_RLY" Source="Universal.TML_RLY.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#4">
    <Task Name="_Menu" Source="HotWire._Menu.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#5">
    <Task Name="HMI_Report" Source="FourAxisTigCtrl.HMI_Report.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#6" />
  <TaskClass Name="Cyclic#7" />
  <TaskClass Name="Cyclic#8">
    <Task Name="SuperMstr" Source="Universal.SuperMstr.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="FileOper" Source="Universal.FileOper.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cJSONTest" Source="UDP.cJSON.cJSONTest.prg" Memory="UserROM" Language="ANSIC" AdditionalLibraryDirectories="\Logical\UDP\sscanf2\" AdditionalLibraries="sscanf2-i386" Debugging="true" />
    <Task Name="udpserv" Source="UDP.paiUDPService.udpserv.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="udpApp" Source="UDP.udpApp.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="udpclient" Source="UDP.paiUDPService.udpclient.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="udpsimonly" Source="UDP.udpsimonly.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <DataObjects>
    <DataObject Name="Acp10sys" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="PermObj_15" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="PermObj_16" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="PermObj_7" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="LogBook1" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="PermObj_3" Source="" Memory="UserROM" Language="Binary" />
  </DataObjects>
  <VcDataObjects>
    <VcDataObject Name="Visu1" Source="Visu1.dob" Memory="UserROM" Language="Vc" WarningLevel="2" />
    <VcDataObject Name="Visu2" Source="Visu2.dob" Memory="UserROM" Language="Vc" WarningLevel="2" />
    <VcDataObject Name="Visu_M" Source="Visu_M.dob" Memory="UserROM" Language="Vc" Disabled="true" WarningLevel="2" />
  </VcDataObjects>
  <Binaries>
    <BinaryObject Name="TCData" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="vccbtn" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdvnc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arialbd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcmgr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcgclass" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcfile" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccnum" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcfntttf" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpkat" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcbclass" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arial" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visvc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcalarm" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcdsloc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vctcal" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfx20" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccshape" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccstr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcshared" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcrt" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcnet" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcctext" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcdsint" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdsw" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccovl" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcchspot" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu202" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu201" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="udbdef" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu203" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccline" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccddbox" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccpopup" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu102" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu101" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu103" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccdt" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfgen" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Acp10map" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="acp10cfg" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="ashwd" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="ashwac" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcclbox" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu_M02" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccbmp" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu_M03" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Visu_M01" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arialbi" Source="" Memory="UserROM" Language="Binary" />
  </Binaries>
  <Libraries>
    <LibraryObject Name="bglibas" Source="Libraries.bglibas.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="asstring" Source="Libraries.asstring.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsXml" Source="Libraries.AsXml.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sprintf2" Source="Libraries.sprintf2.lby" Memory="None" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="AsUDP" Source="Libraries.AsUDP.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Runtime" Source="Libraries.Runtime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10man" Source="Libraries.Acp10man.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10par" Source="Libraries.Acp10par.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="NcGlobal" Source="Libraries.NcGlobal.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="Libraries.sys_lib.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brsystem" Source="Libraries.brsystem.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Library" Source="Libraries.Library.lby" Memory="None" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="dvframe" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="standard" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="fileio" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="asiotime" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="astime" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="dataobj" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="powerlnk" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="astcp" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="vcresman" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="visapi" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="ashw" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="asbrmath" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="ashydcon" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>