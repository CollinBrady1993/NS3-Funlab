/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * NIST-developed software is provided by NIST as a public
 * service. You may use, copy and distribute copies of the software in 
 * any medium, provided that you keep intact this entire notice. You
 * may improve, modify and create derivative works of the software or
 * any portion of the software, and you may copy and distribute such
 * modifications or works. Modified works should carry a notice
 * stating that you changed the software and should note the date and
 * nature of any such change. Please explicitly acknowledge the
 * National Institute of Standards and Technology as the source of the
 * software.
 *
 * NIST-developed software is expressly provided "AS IS." NIST MAKES
 * NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT OR ARISING BY
 * OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * NON-INFRINGEMENT AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR
 * WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED
 * OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT
 * WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE
 * SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE
 * CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 *
 * You are solely responsible for determining the appropriateness of
 * using and distributing the software and you assume all risks
 * associated with its use, including but not limited to the risks and
 * costs of program errors, compliance with applicable laws, damage to
 * or loss of data, programs or equipment, and the unavailability or
 * interruption of operation. This software is not intended to be used
 * in any situation where a failure could cause risk of injury or
 * damage to property. The software developed by NIST employees is not
 * subject to copyright protection within the United States.
 */

#include "rrc-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RrcStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (RrcStatsCalculator);

RrcStatsCalculator::RrcStatsCalculator ()
  : m_discoveryMonitoringRrcFirstWrite (true)
{
  NS_LOG_FUNCTION (this);
}

RrcStatsCalculator::~RrcStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
RrcStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RrcStatsCalculator")
    .SetParent<LteStatsCalculator> ()
    .AddConstructor<RrcStatsCalculator> ()
  ;
  return tid;
}

void 
RrcStatsCalculator::DiscoveryMonitoringRrcTraceCallback (Ptr<RrcStatsCalculator> rrcStats, std::string path, uint64_t imsi, uint16_t cellId, uint16_t rnti, SlDiscMsg discMsg)
{
  NS_LOG_INFO ("Writing Discovery Monitoring Stats in SlDiscRxRrcStats.txt");
  	
  std::ofstream outFile;
  if ( rrcStats->m_discoveryMonitoringRrcFirstWrite == true )
    {
      outFile.open ("SlDiscRxRrcStats.txt");
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file SlDiscRxRrcStats.txt");
          return;
        }
      rrcStats->m_discoveryMonitoringRrcFirstWrite = false;
      outFile << "Time\tIMSI\tCellId\tRNTI\tDiscType\tContentType\tDiscModel\tContent" << std::endl;
    }
  else
    {
      outFile.open ("SlDiscRxRrcStats.txt",  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file SlDiscRxRrcStats.txt");
          return;
        }
    }

	
  outFile << Simulator::Now ().GetSeconds () << "\t" << imsi << "\t" << cellId << "\t" << rnti << "\t";
	
  outFile << (uint16_t) (discMsg.m_msgType >> 6) << "\t" << (uint16_t) ((discMsg.m_msgType >> 2) & 0xF) << "\t" << (uint16_t) (discMsg.m_msgType & 0x3) << "\t";
	
  switch (discMsg.m_msgType)
    {
    case 0x91://UE-to-Network Relay Discovery Announcement in model A
    case 0x92://UE-to-Network Relay Discovery Response in model B
      {  
        std::bitset<24> relayServiceCode(0x0);
        std::bitset<48> announcerInfo(0x0);
        std::bitset<24> proseRelayUeId(0x0);
        std::bitset<8> statusIndicator(0x0);
        std::bitset<80> spare(0x0);
		
        std::memcpy(&relayServiceCode,&discMsg.m_pc5_disc_payload[0],3); 		//Relay Service Code = [0],[1],[2]
        std::memcpy(&announcerInfo,&discMsg.m_pc5_disc_payload[3],6); 			//Announcer Info = User Info = [3],[4],[5],[6],[7],[8]
        std::memcpy(&proseRelayUeId,&discMsg.m_pc5_disc_payload[9],3); 			//ProSe Relay UE ID = [9],[10],[11]
        std::memcpy(&statusIndicator,&discMsg.m_pc5_disc_payload[12],1); 		//Status Indicator = [12]
        std::memcpy(&spare,&discMsg.m_pc5_disc_payload[13],10);					//Spare = [13]..[22]
		
        outFile << std::hex << relayServiceCode.to_ulong() << ";" << std::hex << announcerInfo.to_ulong() << ";" << std::hex << proseRelayUeId.to_ulong() << ";" << std::hex << statusIndicator.to_ulong() << ";" << std::hex << spare.to_ulong() << std::endl;
      }
      break;
    case 0x41:
    case 0x81:
      { //open or restricted announcement
        bool nonzero = false;
        for (uint8_t i =0 ; i < 23 ; i++)
          {
            uint16_t val = discMsg.m_pc5_disc_payload[22-i];
            nonzero = nonzero || (val!=0);
            if (nonzero || i == 22)
              {
                outFile << std::hex << val;
              }
          }
        outFile << std::endl;
      }
      break;
    default:
      NS_LOG_ERROR ("Invalid discovery message type " << discMsg.m_msgType);
    }
}

} // namespace ns3
