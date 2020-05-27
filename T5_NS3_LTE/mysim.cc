/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
//#include "ns3/gtk-config-store.h"

using namespace ns3;

int main (int argc, char *argv[])
{	
  // Create Nodes: eNodeB and UE
 int nueperenb = 20;
  int nenb = 1;
  int distance = 500;
  double eNbTxPower = 30;
  int rrsched = 3;
  int circle = 1;
  int simtime = 20;
  CommandLine cmd;

  //SeedManager::SetSeed(1);
	//SeedManager::SetRun(1);


  // Parse again so you can override default values from the command line
  cmd.AddValue ("NmbUE", "Number of UEs", nueperenb);
  cmd.AddValue ("Distance", "Distance", distance);
  cmd.AddValue ("Sched", "1 for RoundRobin Scheduler, 2 for Max, 3 for default", rrsched);
  cmd.AddValue ("Power", "Power in db", eNbTxPower);
  cmd.AddValue ("Circle", "0 for circle configuration", circle);
  cmd.Parse (argc, argv);


  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (eNbTxPower));

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisPropagationLossModel"));
  // Default scheduler is PF, uncomment to use RR
  switch(rrsched){
      case 1:
        lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
        break;
      case 2:
        lteHelper->SetSchedulerType ("ns3::TdMtFfMacScheduler");
        break;
  }

  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (nenb);
  ueNodes.Create (nueperenb);
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  if(circle == 0){
    float angle = 2*3.14159265/nueperenb;
    for (uint16_t i = 0; i < nueperenb; i++){
      positionAlloc->Add (Vector(distance*sin(angle*i), distance*cos(angle*i), 0));
    }
  }else{
    for (uint16_t i = 0; i < nueperenb; i++){
        positionAlloc->Add (Vector(0, distance, 0));
    }
  }

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install (ueNodes);
  BuildingsHelper::Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  // Default scheduler is PF, uncomment to use RR
  //lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");

  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  for (uint16_t i = 0; i < nueperenb; i++){
    lteHelper->Attach (ueDevs.Get(i), enbDevs.Get (0));
  }


  // Activate a data radio bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

lteHelper->EnablePhyTraces ();
lteHelper->EnableMacTraces ();
lteHelper->EnableRlcTraces ();
lteHelper->EnablePdcpTraces ();

  Simulator::Stop (Seconds (simtime));

  Simulator::Run ();

  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  Simulator::Destroy ();
  return 0;
}