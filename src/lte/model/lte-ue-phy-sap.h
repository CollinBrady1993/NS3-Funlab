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
 * Author: Marco Miozzo <mmiozzo@cttc.es>
 * Modified by: NIST // Contributions may not be subject to US copyright.
 */



#ifndef LTE_UE_PHY_SAP_H
#define LTE_UE_PHY_SAP_H

#include <ns3/packet.h>

namespace ns3 {

class LteControlMessage;

/**
* Service Access Point (SAP) offered by the UE-PHY to the UE-MAC
*
* This is the PHY SAP Provider, i.e., the part of the SAP that contains
* the PHY methods called by the MAC
*/
class LteUePhySapProvider
{
public:
  virtual ~LteUePhySapProvider ();
  
  /**
   * Parameters for LteUePhySapProvider::TransmitPhySdu
   */
  struct TransmitSlPhySduParameters
  {
    enum SidelinkChannel
      {
        PSBCH,
          PSCCH,
          PSSCH,
          PSDCH
      } m_channel;      
    //Common information
    uint8_t   m_rbStart; ///< models rb assignment
    uint8_t   m_rbLen;   ///< models rb assignment

    //PSCCH, PSDCH
    uint32_t m_resNo; ///< the resource index from the communication/discovery pool 

    //PSDH and PSSCH information
    uint8_t   m_rv;       ///< indicates HARQ revision number
    
    //PSSCH information
    uint8_t   m_hopping; ///< hopping flag
    uint8_t   m_hoppingInfo; ///< models rb assignment when hopping is enabled
    uint8_t   m_dstId;   ///< layer 1 destination
  };

  /**
  * \brief Send The MAC PDU to the channel
  * \param p The MAC PDU to send
  */
  virtual void SendMacPdu (Ptr<Packet> p) = 0;

  /**
  * \brief Send a sidelink MAC PDU to the channel
  * \param p The sidelink MAC PDU
  * \param params Additional information about the PDU
  */
  virtual void SendSlMacPdu (Ptr<Packet> p, TransmitSlPhySduParameters params) = 0;

  /**
  * \brief Send SendLteControlMessage (PDCCH map, CQI feedbacks) using the ideal control channel
  * \param msg The Ideal Control Message to send
  */
  virtual void SendLteControlMessage (Ptr<LteControlMessage> msg) = 0;

  /** 
   * Send a preamble on the PRACH
   * 
   * \param prachId the ID of the preamble
   * \param raRnti the RA RNTI
   */
  virtual void SendRachPreamble (uint32_t prachId, uint32_t raRnti) = 0;

  //Sidelink Communication

  /**
   * Set grant for discovery
   * \param resPsdch The resource to use in the discovery pool
   */
  virtual void SetDiscGrantInfo (uint8_t resPsdch) = 0;

};


/**
* Service Access Point (SAP) offered by the PHY to the MAC
*
* This is the PHY SAP User, i.e., the part of the SAP that contains the MAC
* methods called by the PHY
*/
class LteUePhySapUser
{
public:
  virtual ~LteUePhySapUser ();


  /**
   * Called by the Phy to notify the MAC of the reception of a new PHY-PDU
   *
   * \param p The packet
   */
  virtual void ReceivePhyPdu (Ptr<Packet> p) = 0;

  /**
   * Called by the Phy to notify the MAC of the reception of a new PHY-PDU
   *
   * \param p The packet
   */
  virtual void ReceiveSlDiscPhyPdu (Ptr<Packet> p) = 0;

  /**
   * Called by the Phy to notify the MAC of the reception of a new PHY-PDU
   *
   * \param p The packet
   */
  virtual void ReceiveSlSciPhyPdu (Ptr<Packet> p) = 0;

  /**
   * \brief Trigger the start from a new frame (input from Phy layer)
   * \param frameNo The frame number
   * \param subframeNo The subframe number
   */
  virtual void SubframeIndication (uint32_t frameNo, uint32_t subframeNo) = 0;

  /**
   * \brief Receive SendLteControlMessage (PDCCH map, CQI feedbacks) using the ideal control channel
   * \param msg The Ideal Control Message to receive
   */
  virtual void ReceiveLteControlMessage (Ptr<LteControlMessage> msg) = 0;

  /**
   * Notify the MAC that the PHY changed of timing as consequence of a change of SyncRef
   * Adjust the MAC subframe indication
   *
   * \param frameNo The current PHY frame number
   * \param subframeNo The current PHY subframe number
   */
   virtual void NotifyChangeOfTiming (uint32_t frameNo, uint32_t subframeNo) = 0;

  /**
   * Notify the MAC that Sidelink is configured
   */
  virtual void NotifySidelinkEnabled () = 0;
};

} // namespace ns3


#endif // LTE_UE_PHY_SAP_H
