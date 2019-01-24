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
 * Author: Author: Marco Miozzo  <marco.miozzo@cttc.es>
 * Modified by: NIST // Contributions may not be subject to US copyright.
 */
#ifndef LTE_SL_TAG_H
#define LTE_SL_TAG_H

#include "ns3/tag.h"
#include "ns3/nstime.h"

namespace ns3 {

class Tag;


/**
 * Tag used to define the resource number in the control pool and the
 * transport block of the shared channel
 */

class LteSlSciTag : public Tag
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create an empty LteSlSciTag
   */
  LteSlSciTag ();

  /**
   * Create a LteSlSciTag with the given resource number and transport block size
   * \param rnti The RNTI
   * \param resNo The resource number
   * \param tbSize The transport block size
   */
  LteSlSciTag (uint16_t rnti, uint16_t resNo, uint32_t tbSize);
  

  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual uint32_t GetSerializedSize () const;
  virtual void Print (std::ostream &os) const;

  /**
   * Get RNTI function
   *
   * \returns RNTI
   */
  uint16_t GetRnti (void) const;
  /**
   * Get LCID function
   *
   * \returns LCID
   */
  uint16_t GetResNo (void) const;
  /**
   * Get layer function
   *
   * \returns layer
   */
  uint32_t GetTbSize (void) const;

private:
  uint16_t m_rnti; ///< RNTI
  uint16_t m_resNo; ///< Resource number
  uint32_t m_tbSize; ///< Transport block size

};


/**
 * Tag used to define the resource number in the control pool and the
 * transport block of the shared channel
 */

class LteMibSlTag : public Tag
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create an empty LteMibSlTag
   */
  LteMibSlTag ();

  /**
   * Create a LteMibSlTag with the given creation timestamp
   * \param t The creation time of the tag
   */
  LteMibSlTag (Time t);
  

  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual uint32_t GetSerializedSize () const;
  virtual void Print (std::ostream &os) const;

  /**
   * Sets the Rx offset
   *
   * \param offset The reception offset
   */
  void SetRxOffset (const uint16_t offset);
  
  /**
   * Sets the slssid
   * 
   * \param slssid The SLSS ID
   */
  void SetSlssid (const uint64_t slssid);
  
  /**
   * Get creation timestamp function
   *
   * \returns RNTI
   */
  Time GetCreationTimestamp (void) const;
  
  /**
   * Get creation timestamp function
   *
   * \returns RNTI
   */
  Time GetRxTimestamp (void) const;
  
  /**
   * Get layer function
   *
   * \returns layer
   */
  uint16_t GetRxOffset (void) const;

private:
    Time m_creationTimestamp;   ///< Creation timestamp filled when created
    Time m_rxTimestamp;   ///< Reception timestamp filled upon reception
    uint16_t m_rxOffset;   ///< Reception offset
    uint64_t  m_slssid;   ///< SLSSID of the SyncRef sending the MIB-SL

};


} // namespace ns3

#endif /* LTE_SL_TAG_H */
