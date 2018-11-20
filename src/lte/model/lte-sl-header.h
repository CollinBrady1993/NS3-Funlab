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

#ifndef LTE_SL_HEADER_H
#define LTE_SL_HEADER_H

#include "ns3/header.h"
#include <ns3/ff-mac-common.h>

#include <list>

namespace ns3 {

/**
 * \ingroup lte
 * \brief The packet header for the discovery packets
 *
 */
class LteSlDiscHeader : public Header
{
public:
  enum DiscoveryMsgType : uint8_t
  {
    DISC_OPEN_ANNOUNCEMENT = 0x41, /* Open discovery announce model A */
    DISC_RESTRICTED_ANNOUNCEMENT = 0x81, /* Restricted discovery announce model A */
    DISC_RESTRICTED_RESPONSE = 0x82, /* Restricted discovery response model B */
    DISC_RELAY_ANNOUNCEMENT = 0x91, /* Relay Discovery Announcement in model A */
    DISC_RELAY_SOLICITATION = 0x96, /* Relay Discovery Announcement in model A */
    DISC_RELAY_RESPONSE = 0x92, /* UE-to-Network Relay Discovery Response in model B */
  };

  /**
   * \brief Constructor
   *
   * Creates a null header
   */
  LteSlDiscHeader ();
  ~LteSlDiscHeader ();

  void SetMic (uint32_t mic);
  void SetUtcBaseCounder (uint8_t counter);

  uint8_t GetMessageType () const;
  uint32_t GetMic () const;
  uint8_t GetUtcBaseCounter () const;

  uint64_t GetApplicationCode () const;
  uint32_t GetRelayServiceCode () const;
  uint64_t GetInfo () const;
  uint32_t GetRelayUeId () const;
  uint8_t GetStatusIndicator () const;
  uint8_t GetURDSCommposition () const;
  uint32_t GetGroup () const;
  uint32_t GetGroupInfo () const;


  void SetOpenDiscoveryAnnounceParameters (uint64_t appCode);
  void SetRestrictedDiscoveryAnnounceParameters (uint64_t appCode);

  void SetRelayAnnouncementParameters (uint32_t serviceCode, uint64_t announcerInfo, uint32_t relayUeId, uint32_t status);
  void SetRelaySoliciationParameters (uint32_t serviceCode, uint64_t discovererInfo, uint32_t relayUeId);
  void SetRelayResponseParameters (uint32_t serviceCode, uint64_t discovereeInfo, uint32_t relayUeId, uint32_t status);


  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  /* Message Type variable shall have the following values:
     65 for ProSe Open Discovery announce for Model A.
     145 for ProSe Restricted UE-to-Network Relay Discovery announce for Model A. */
  uint8_t m_msgType;

  uint64_t m_appCode;
  uint32_t m_relayServiceCode;
  uint64_t m_info; //announcer, or discoverer, or discoveree, or user
  uint32_t m_relayUeId;
  uint8_t m_statusIndicator;

  //variables for group member discovery
  uint32_t m_group;
  uint32_t m_groupInfo;

  uint32_t m_mic;
  uint8_t m_utcBasedCounter;

};

} // namespace ns3

#endif // LTE_SL_HEADER_H
