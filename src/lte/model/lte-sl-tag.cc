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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 * Modified by: NIST // Contributions may not be subject to US copyright.
 */


#include "lte-sl-tag.h"
#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteSlSciTag);

TypeId
LteSlSciTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteSlSciTag")
    .SetParent<Tag> ()
    .SetGroupName("Lte")
    .AddConstructor<LteSlSciTag> ()
    .AddAttribute ("rnti", "The rnti that indicates the UE to which packet belongs",
                   UintegerValue (0),
                   MakeUintegerAccessor (&LteSlSciTag::GetRnti),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("resNo", "The resource number within the control pool",
                   UintegerValue (0),
                   MakeUintegerAccessor (&LteSlSciTag::GetResNo),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("tbSize", "The transport block size",
                   UintegerValue (0),
                   MakeUintegerAccessor (&LteSlSciTag::GetTbSize),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

TypeId
LteSlSciTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

LteSlSciTag::LteSlSciTag ()
  : m_rnti (0),
    m_resNo (0),
    m_tbSize (0)
{
}
LteSlSciTag::LteSlSciTag (uint16_t rnti, uint16_t resNo, uint32_t tbSize)
  : m_rnti (rnti),
    m_resNo (resNo),
    m_tbSize (tbSize)
{
}
  
uint32_t
LteSlSciTag::GetSerializedSize (void) const
{
 return 8;
}

void
LteSlSciTag::Serialize (TagBuffer i) const
{
  i.WriteU16 (m_rnti);
  i.WriteU16 (m_resNo);
  i.WriteU32 (m_tbSize);
}

void
LteSlSciTag::Deserialize (TagBuffer i)
{
  m_rnti = (uint16_t) i.ReadU16 ();
  m_resNo = (uint8_t) i.ReadU16 ();
  m_tbSize = (uint8_t) i.ReadU32 ();
}

uint16_t
LteSlSciTag::GetRnti () const
{
  return m_rnti;
}

uint16_t
LteSlSciTag::GetResNo () const
{
  return m_resNo;
}

uint32_t
LteSlSciTag::GetTbSize () const
{
  return m_tbSize;
}

void
LteSlSciTag::Print (std::ostream &os) const
{
  os << "rnti=" << m_rnti << ", resNo=" << (uint16_t) m_resNo << ", tbSize=" << m_tbSize;
}

/////////////////// LteMibSlTag ////////////////

TypeId
LteMibSlTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteMibSlTag")
    .SetParent<Tag> ()
    .SetGroupName("Lte")
    .AddConstructor<LteSlSciTag> ()
  ;
  return tid;
}

TypeId
LteMibSlTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

LteMibSlTag::LteMibSlTag ()
  : m_creationTimestamp (0),
    m_slssid (0)
{
}
 
LteMibSlTag::LteMibSlTag (Time t)
{
  m_creationTimestamp = t;
}

uint32_t
LteMibSlTag::GetSerializedSize (void) const
{
 return 10;
}

void
LteMibSlTag::Serialize (TagBuffer i) const
{
  int64_t t = m_creationTimestamp.GetNanoSeconds ();
  i.Write ((const uint8_t *)&t, 8);
  i.WriteU16 (m_slssid);
}

void
LteMibSlTag::Deserialize (TagBuffer i)
{
  int64_t t;
  i.Read ((uint8_t *)&t, 8);
  m_creationTimestamp = NanoSeconds (t);
  m_slssid = (uint64_t) i.ReadU16 ();
}

void 
LteMibSlTag::SetSlssid (const uint16_t slssid)
{
  m_slssid = slssid;
}

Time
LteMibSlTag::GetCreationTimestamp () const
{
  return m_creationTimestamp;
}

uint16_t
LteMibSlTag::GetSlssid () const
{
  return m_slssid;
}

void
LteMibSlTag::Print (std::ostream &os) const
{
  os << "slssid " << m_slssid << "timestamp " << m_creationTimestamp;
}



} // namespace ns3
