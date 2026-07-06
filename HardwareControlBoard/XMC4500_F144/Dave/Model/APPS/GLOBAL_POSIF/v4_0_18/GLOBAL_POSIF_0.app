<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="GLOBAL_POSIF" URI="http://resources/4.0.18/app/GLOBAL_POSIF/0" description="Configures the POSIF module global settings." version="4.0.18" minDaveVersion="4.0.0" instanceLabel="GLOBAL_POSIF_0" appLabel="">
  <upwardMapList xsi:type="ResourceModel:RequiredApp" href="../../ENCODER_POSIF/v4_0_6/ENCODER_POSIF_0.app#//@requiredApps.0"/>
  <upwardMapList xsi:type="ResourceModel:RequiredApp" href="../../ENCODER_POSIF/v4_0_6/ENCODER_POSIF_1.app#//@requiredApps.0"/>
  <properties provideInit="true" sharable="true"/>
  <virtualSignals name="posif_global_signal" URI="http://resources/4.0.18/app/GLOBAL_POSIF/0/vs_global_posif_globalsignal" hwSignal="global_signal" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="posif_clock_signal" URI="http://resources/4.0.18/app/GLOBAL_POSIF/0/vs_global_posif_clk" hwSignal="clk" hwResource="//@hwResources.0" visible="true"/>
  <requiredApps URI="http://resources/4.0.18/app/GLOBAL_POSIF/0/appres_clock_0" requiredAppName="CLOCK_XMC4" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../CLOCK_XMC4/v4_0_24/CLOCK_XMC4_0.app#/"/>
  </requiredApps>
  <hwResources name="Posif Global" URI="http://resources/4.0.18/app/GLOBAL_POSIF/0/hwres_posif_global_0" resourceGroupUri="peripheral/posif/*/posif/global" mResGrpUri="peripheral/posif/*/posif/global">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/POSIF0/POSIF0_0.dd#//@provided.24"/>
  </hwResources>
</ResourceModel:App>
