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

#include "ns3/log.h"

#include "ns3/lte-sl-header.h"
#include "lte-sl-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteSlHeader");

NS_OBJECT_ENSURE_REGISTERED (LteSlDiscHeader);

LteSlDiscHeader::LteSlDiscHeader ()
  : m_msgType (0),
  m_appCode (0),
  m_relayServiceCode (0),
  m_info (0),
  m_relayUeId (0),
  m_statusIndicator (0),
  m_group (0),
  m_groupInfo (0),
  m_mic (0),
  m_utcBasedCounter (0)
{


}

LteSlDiscHeader::~LteSlDiscHeader ()
{
  m_msgType = 0;
}

void
LteSlDiscHeader::SetOpenDiscoveryAnnounceParameters (uint64_t appCode)
{
  m_msgType = DISC_OPEN_ANNOUNCEMENT;
  m_appCode = appCode;
}
void
LteSlDiscHeader::SetRestrictedDiscoveryAnnounceParameters (uint64_t appCode)
{
  m_msgType = DISC_RESTRICTED_ANNOUNCEMENT;
  m_appCode = appCode;
}

void
LteSlDiscHeader::SetRelayAnnouncementParameters (uint32_t serviceCode, uint64_t announcerInfo, uint32_t relayUeId, uint32_t status)
{
  m_msgType = DISC_RELAY_ANNOUNCEMENT;
  m_relayServiceCode = serviceCode;
  m_info = announcerInfo;
  m_relayUeId = relayUeId;
  m_statusIndicator = status;
}
void
LteSlDiscHeader::SetRelaySoliciationParameters (uint32_t serviceCode, uint64_t discovererInfo, uint32_t relayUeId)
{
  m_msgType = DISC_RELAY_SOLICITATION;
  m_relayServiceCode = serviceCode;
  m_info = discovererInfo;
  m_relayUeId = relayUeId;
}
void
LteSlDiscHeader::SetRelayResponseParameters (uint32_t serviceCode, uint64_t discovereeInfo, uint32_t relayUeId, uint32_t status)
{
  m_msgType = DISC_RELAY_RESPONSE;
  m_relayServiceCode = serviceCode;
  m_info = discovereeInfo;
  m_relayUeId = relayUeId;
  m_statusIndicator = status;
}



void
LteSlDiscHeader::SetMic (uint32_t mic)
{
  m_mic = mic;
}

void
LteSlDiscHeader::SetUtcBaseCounder (uint8_t counter)
{
  m_utcBasedCounter = counter;
}

uint64_t
LteSlDiscHeader::GetApplicationCode () const
{
  return m_appCode;
}
uint32_t
LteSlDiscHeader::GetRelayServiceCode () const
{
  return m_relayServiceCode;
}
uint64_t
LteSlDiscHeader::GetInfo () const
{
  return m_info;
}
uint32_t
LteSlDiscHeader::GetRelayUeId () const
{
  return m_relayUeId;
}
uint8_t
LteSlDiscHeader::GetStatusIndicator () const
{
  return m_statusIndicator;
}

uint8_t
LteSlDiscHeader::GetURDSCommposition () const
{
  if (m_relayUeId != 0)
    {
      return 1;
    }
  return 0;
}

uint32_t
LteSlDiscHeader::GetGroup () const
{
  return m_group;
}
uint32_t
LteSlDiscHeader::GetGroupInfo () const
{
  return m_groupInfo;
}


uint32_t
LteSlDiscHeader::GetMic () const
{
  return m_mic;
}

uint8_t
LteSlDiscHeader::GetUtcBaseCounter () const
{
  return m_utcBasedCounter;
}


uint8_t
LteSlDiscHeader::GetMessageType () const
{
  return m_msgType;
}


TypeId
LteSlDiscHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteSlDiscHeader")
    .SetParent<Header> ()
    .AddConstructor<LteSlDiscHeader> ()
  ;
  return tid;
}

TypeId
LteSlDiscHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
LteSlDiscHeader::Print (std::ostream &os)  const
{
  //todo
}

uint32_t
LteSlDiscHeader::GetSerializedSize (void) const
{
  return 29;
}

void
LteSlDiscHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_msgType);
  uint8_t padding = 0;

  switch (m_msgType)
    {
    case DISC_OPEN_ANNOUNCEMENT:
    case DISC_RESTRICTED_ANNOUNCEMENT:
      i.WriteU64 (m_appCode);
      i.WriteU8 ( padding, 15);
      break;
    case DISC_RESTRICTED_RESPONSE:
      break;
    case DISC_RELAY_ANNOUNCEMENT:
    case DISC_RELAY_RESPONSE:
      i.WriteU16 (m_relayServiceCode & 0xFFFF);
      i.WriteU8 ((m_relayServiceCode >> 16) & 0xFF);
      i.WriteU32 (m_info & 0xFFFFFFFF);
      i.WriteU16 ((m_info >> 32) & 0xFFFF);
      i.WriteU16 (m_relayUeId & 0xFFFF);
      i.WriteU8 ((m_relayUeId >> 16) & 0xFF);
      i.WriteU8 (m_statusIndicator);
      i.WriteU8 ( padding, 10);
      break;
    case DISC_RELAY_SOLICITATION:
      i.WriteU16 (m_relayServiceCode & 0xFFFF);
      i.WriteU8 ((m_relayServiceCode >> 16) & 0xFF);
      i.WriteU32 (m_info & 0xFFFFFFFF);
      i.WriteU16 ((m_info >> 32) & 0xFFFF);
      if (m_relayUeId == 0)
        {
          i.WriteU8 (0);
          i.WriteU8 ( padding, 13);
        }
      else
        {
          i.WriteU8 (1);
          i.WriteU16 (m_relayUeId & 0xFFFF);
          i.WriteU8 ((m_relayUeId >> 16) & 0xFF);
          i.WriteU8 ( padding, 10);
        }

    default:
      break;
    }

  i.WriteU32 (m_mic);
  i.WriteU8 (m_utcBasedCounter);
}

uint32_t
LteSlDiscHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_msgType = i.ReadU8 ();
  uint8_t padding[15];
  uint64_t tmp;
  switch (m_msgType)
    {
    case DISC_OPEN_ANNOUNCEMENT:
    case DISC_RESTRICTED_ANNOUNCEMENT:
      m_appCode = i.ReadU64 ();
      i.Read (padding, 15);
      break;
    case DISC_RESTRICTED_RESPONSE:
      break;
    case DISC_RELAY_ANNOUNCEMENT:
    case DISC_RELAY_RESPONSE:
      m_relayServiceCode = i.ReadU16 ();
      m_relayServiceCode += i.ReadU8 () << 16;
      m_info = i.ReadU32 ();
      tmp = i.ReadU16 ();
      m_info += tmp << 32;
      m_relayUeId = i.ReadU16 ();
      m_relayUeId += i.ReadU8 () << 16;
      m_statusIndicator = i.ReadU8 ();
      i.Read (padding, 10);
      break;
    case DISC_RELAY_SOLICITATION:
      m_relayServiceCode = i.ReadU16 ();
      m_relayServiceCode += i.ReadU8 () << 16;
      m_info = i.ReadU32 ();
      tmp = i.ReadU16 ();
      m_info += tmp << 32;
      
      if (i.ReadU8 () == 0)
        {
          i.Read (padding, 13);
        }
      else
        {
          m_relayUeId = i.ReadU16 ();
          m_relayUeId += i.ReadU8 () << 16;
          i.Read (padding, 10);
        }

    default:
      break;
    }
  
  
  m_mic = i.ReadU32 ();
  m_utcBasedCounter = i.ReadU8 ();

  return GetSerializedSize ();
}

}  // namespace ns3
