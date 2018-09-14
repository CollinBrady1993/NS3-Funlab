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

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteSlHeader");

NS_OBJECT_ENSURE_REGISTERED (LteSlDiscHeader);

LteSlDiscHeader::LteSlDiscHeader ()
  : m_msgType (0),
    m_mic (0),
    m_utcBasedCounter (0)
{
  
  
}

LteSlDiscHeader::~LteSlDiscHeader ()
{
   m_msgType = 0;
}

void
LteSlDiscHeader::SetMessageType (uint8_t msgType)
{
  m_msgType = msgType;
}

void 
LteSlDiscHeader::SetPayload (uint8_t payload[23])
{
  std::memcpy (m_pc5_disc_payload, payload, 23);
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

void
LteSlDiscHeader::GetPayload (uint8_t *payload) const 
{
  
  std::memcpy (payload, m_pc5_disc_payload, 23);;
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

  for (uint32_t j = 0 ; j < 23 ; j++)
    {
      i.WriteU8 (m_pc5_disc_payload[j]);
    }
  
  i.WriteU32 (m_mic);
  i.WriteU8 (m_utcBasedCounter);
}

uint32_t
LteSlDiscHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_msgType = i.ReadU8();
  for (uint32_t j = 0 ; j < 23 ; j++)
    {
      m_pc5_disc_payload[j] = i.ReadU8 ();
    }
  m_mic = i.ReadU32 ();
  m_utcBasedCounter = i.ReadU8 ();
  
  return GetSerializedSize ();
}

}; // namespace ns3
