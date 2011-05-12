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

#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"

#include "ns3/mobility-helper.h"
#include "ns3/lena-helper.h"

#include "ns3/lte-test-link-adaptation.h"

NS_LOG_COMPONENT_DEFINE ("LteLinkAdaptationTest");

using namespace ns3;


/**
 * Test 1.3 Link Adaptation
 */

void
LteTestDlSchedulingCallback (LteLinkAdaptationTestCase *testcase, std::string path,
                            uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                            uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
    testcase->DlScheduling(frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}

/**
 * TestSuite
 */

LteLinkAdaptationTestSuite::LteLinkAdaptationTestSuite ()
  : TestSuite ("lte-link-adaptation", SYSTEM)
{
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
//   LogComponentEnable ("LteLinkAdaptationTest", logLevel);

  NS_LOG_INFO ("Creating LteLinkAdaptionTestSuite");

// Tests: distance --> loss
#if 0
  int distanceMin = 1;
  int distanceMax = 2000;
  int distanceStep = 1000;
  bool logStep = false;

  for ( int distance = distanceMin ;
        distance <= distanceMax ;
        logStep ? ( distance *= distanceStep) : ( distance += distanceStep ) )
    {
      /**
       * Propagation Loss (in W/Hz)
       *
       *         (  4 * PI * distance * frequency  ) 2
       *  Loss = ( ------------------------------- )
       *         (            c                    )
       *
       *  where: c is speed of light in vacuum = 3e8 (m/s)
       *         distance in (m)
       *         frequency in (Hz)
       */
      double lossLinear = ( ( 4.0 * M_PI * distance * 2.160e9 ) / 3e8 );
      lossLinear = lossLinear * lossLinear;
      double lossDb = 10 * log10(lossLinear);

      AddTestCase (new LteLinkAdaptationTestCase (lossDb, distance));
    }
#endif

  struct SnrEfficiencyMcs
    {
      double  snr;
      double  efficiency;
      int  mcsIndex;
    };

  /**
    * Test vectors: SNR, Spectral Efficiency, MCS index
    * From XXX
    */
  SnrEfficiencyMcs snrEfficiencyMcs[] = {
    // {-5.00000,	0.08024,	-1},
    // {-4.00000,	0.10030,	-1},
    // {-3.00000,	0.12518,	-1},
    {-2.00000,	0.15589,	0},
    {-1.00000,	0.19365,	0},
    {0.00000,	0.23983,	2},
    {1.00000,	0.29593,	2},
    {2.00000,	0.36360,	2},
    {3.00000,	0.44451,	4},
    {4.00000,	0.54031,	4},
    {5.00000,	0.65251,	6},
    {6.00000,	0.78240,	6},
    {7.00000,	0.93086,	8},
    {8.00000,	1.09835,	8},
    {9.00000,	1.28485,	10},
    {10.00000,	1.48981,	12},
    {11.00000,	1.71229,	12},
    {12.00000,	1.95096,	14},
    {13.00000,	2.20429,	14},
    {14.00000,	2.47062,	16},
    {15.00000,	2.74826,	18},
    {16.00000,	3.03560,	18},
    {17.00000,	3.33115,	20},
    {18.00000,	3.63355,	20},
    {19.00000,	3.94163,	22},
    {20.00000,	4.25439,	22},
    {21.00000,	4.57095,	24},
    {22.00000,	4.89060,	24},
    {23.00000,	5.21276,	26},
    {24.00000,	5.53693,	26},
    {25.00000,	5.86271,	28},
    {26.00000,	6.18980,	28},
    {27.00000,	6.51792,	28},
    {28.00000,	6.84687,	28},
    {29.00000,	7.17649,	28},
    {30.00000,	7.50663,	28},
  };   
  int numOfTests = sizeof (snrEfficiencyMcs) / sizeof (SnrEfficiencyMcs);


  for ( int i = 0 ; i < numOfTests; i++ )
    {
      /**
       * SNR (in dB)
       *
       *  SNR = P_tx - loss - noise - receiverNoiseFigure
       *
       *  loss = P_tx - SNR - noise - receiverNoiseFigure
       *
       *  where: P_tx is transmission power
       *         loss in (dB)
       *         noise
       */
      double noiseDb = -107.5;
      double receiverNoiseFigureDb = 5.0;
      double lossDb = 30.0 - snrEfficiencyMcs[i].snr - noiseDb - receiverNoiseFigureDb;
      double lossLinear = pow (10, lossDb / 10);
      double distance = ( ( 3e8 * sqrt ( lossLinear ) ) / ( 4.0 * M_PI * 2.160e9 ) );

      std::ostringstream name;
      name << "link adaptation"
           << " snr= " << snrEfficiencyMcs[i].snr
           << " mcs= " << snrEfficiencyMcs[i].mcsIndex;
      AddTestCase (new LteLinkAdaptationTestCase (name.str (),  snrEfficiencyMcs[i].snr, lossDb, distance, snrEfficiencyMcs[i].mcsIndex));
    }

}

static LteLinkAdaptationTestSuite lteLinkAdaptationTestSuite;


/**
 * TestCase
 */

LteLinkAdaptationTestCase::LteLinkAdaptationTestCase (std::string name, double snr, double loss, double distance, uint16_t mcsIndex)
  : TestCase (name),
    m_snr (snr),
    m_loss (loss),
    m_distance (distance),
    m_mcsIndex (mcsIndex)
{
  std::ostringstream sstream1, sstream2;
  sstream1 << " snr=" << snr 
           << " mcs=" << mcsIndex;

  NS_LOG_UNCOND ("Creating LteLinkAdaptationTestCase: " + sstream1.str ());
}

LteLinkAdaptationTestCase::~LteLinkAdaptationTestCase ()
{
}

void
LteLinkAdaptationTestCase::DoRun (void)
{
   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
   LogComponentEnable ("LteAmc", logLevel);
   LogComponentEnable ("LteLinkAdaptationTest", logLevel);
//   LogComponentEnable ("LteEnbRrc", logLevel);
//   LogComponentEnable ("LteUeRrc", logLevel);
//   LogComponentEnable ("LteEnbMac", logLevel);
//   LogComponentEnable ("LteUeMac", logLevel);
//   LogComponentEnable ("LteRlc", logLevel);
//   LogComponentEnable ("RrPacketScheduler", logLevel);

//   LogComponentEnable ("RrFfMacScheduler", logLevel);

//   LogComponentEnable ("LtePhy", logLevel);
//   LogComponentEnable ("LteEnbPhy", logLevel);
//   LogComponentEnable ("LteUePhy", logLevel);

//   LogComponentEnable ("LteSpectrumValueHelper", logLevel);
//   LogComponentEnable ("LteSpectrumPhy", logLevel);
//   LogComponentEnable ("LteInterference", logLevel);
//   LogComponentEnable ("LteSinrChunkProcessor", logLevel);

//   LogComponentEnable ("LtePropagationLossModel", logLevel);
//   LogComponentEnable ("LossModel", logLevel);
//   LogComponentEnable ("ShadowingLossModel", logLevel);
//   LogComponentEnable ("PenetrationLossModel", logLevel);
//   LogComponentEnable ("MultipathLossModel", logLevel);
//   LogComponentEnable ("PathLossModel", logLevel);
//   LogComponentEnable ("FriisSpectrumPropagationLossModel", logLevel);
//   LogComponentEnable ("ConstantSpectrumPropagationLossModel", logLevel);

//   LogComponentEnable ("LteNetDevice", logLevel);
//   LogComponentEnable ("LteUeNetDevice", logLevel);
//   LogComponentEnable ("LteEnbNetDevice", logLevel);

//   LogComponentEnable ("MobilityModel", logLevel);
//   LogComponentEnable ("ConstantPositionMobilityModel", logLevel);
//   LogComponentEnable ("MultiModelSpectrumChannel", logLevel);
//   LogComponentEnable ("SingleModelSpectrumChannel", logLevel);

  /**
    * Simulation Topology
    */

  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
//   lena->EnableLogComponents ();
  lena->EnableMacTraces ();
  lena->EnableRlcTraces ();
  lena->SetAttribute ("PropagationModel", StringValue ("ns3::ConstantSpectrumPropagationLossModel"));
  NS_LOG_INFO ("SNR = " << m_snr << "  LOSS = " << m_loss);
  lena->SetPropagationModelAttribute ("Loss", DoubleValue (m_loss));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes );

  // Position of the eNodeB and UE
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (100.0, 0.0, 0.0));

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);


  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/DlScheduling",
                    MakeBoundCallback(&LteTestDlSchedulingCallback, this));

   Simulator::Stop (Seconds (0.005));
   //Simulator::Stop (Seconds (0.01));
//   Simulator::Stop (Seconds (0.1));
/*  Simulator::Stop (Seconds (2.0));*/
//   Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
}


void
LteLinkAdaptationTestCase::DlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                         uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  static bool firstTime = true;

  if ( firstTime )
    {
      firstTime = false;
//       NS_LOG_UNCOND (" Frame\tSbframe\tRnti\tmcsTb1\tsizeTb1\tmcsTb2\tsizeTb2");
      NS_LOG_UNCOND ("SNR\tRef_MCS\tCalc_MCS");
    }

  /**
   * Note:
   *    For first 4 subframeNo in the first frameNo, the MCS cannot be properly evaluated,
   *    because CQI feedback is still not available at the eNB.
   */
  if ( (frameNo > 1) || (subframeNo > 4) )
//   if ( (frameNo == 1) && (subframeNo == 10) )
    {
//       NS_LOG_UNCOND (" " << frameNo << "\t" << subframeNo << "\t" << rnti << "\t"
//                         << (uint16_t)mcsTb1 << "\t" << sizeTb1 << "\t"
//                         << (uint16_t)mcsTb2 << "\t" << sizeTb2);

      NS_LOG_UNCOND (m_snr << "\t" << m_mcsIndex << "\t" << (uint16_t)mcsTb1);

      NS_TEST_ASSERT_MSG_EQ ((uint16_t)mcsTb1, m_mcsIndex, "Wrong MCS index");
    }
}