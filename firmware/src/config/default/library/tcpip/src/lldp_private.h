/*******************************************************************************
 LLDP Private API Header File
 
  Company:
    Microchip Technology Inc.
    
  File Name:
    lldp_private.h
    
  Summary:

  Description:

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*
Copyright (C) 2012-2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/








// DOM-IGNORE-END

#ifndef _LLDP_PRIVATE_H
#define _LLDP_PRIVATE_H

#define _LLDP_DEBUG


//***************
// Private types
//***************

#define CISCO_OUI   0x000142
#define IEEE_3_OUI  0x00120F
#define IEEE_1_OUI  0x0080C2
#define TIA_OUI     0x0012bb

// a unique identifier length, in bytes
#define ORG_SPECIFIC_OUI_LENGTH  3

// RX status
//

/**********************LLDP RX TIMER*************************/
typedef struct
{
    uint16_t tooManyNeighborsTimer;     /**< IEEE 802.1AB 9.2.5.14*/
    uint16_t rxTTL;                     /**< IEEE 802.1AB 9.2.5.13*/
}lldp_rx_timers_t;

/**************LLDP RECEIVE STATE MACHINE*****************/

typedef struct       /**< IEEE 802.1AB 9.2.6*/
{      
    uint32_t statsAgeoutsTotal;
    uint32_t statsFramesDiscardedTotal;
    uint32_t statsFramesInErrorsTotal;
    uint32_t statsFramesInTotal;
    uint32_t statsTLVsDiscardedTotal;
    uint32_t statsTLVsUnrecognizedTotal;
    uint32_t lldpuLengthErrors;
}lldp_rx_stats_t;


typedef struct
{
    uint8_t *frame;
    //ssize_t recvsize;
    uint8_t state;
    bool badFrame;
    bool rcvFrame;
    //uint8_t rxChanges; /* This belongs in the MSAP cache */
    bool rxInfoAge;   /**< IEEE 802.1AB 9.2.5.13*/
    bool somethingChangedRemote;
    bool tooManyNeighbors;
    lldp_rx_timers_t timers;
    lldp_rx_stats_t rxStats;
  //    struct lldp_msap_cache *msap;
}lldp_rx_port_t ;


typedef enum 
{
    RX_IDLE,
    LLDP_WAIT_PORT_OPERATIONAL,
    DELETE_AGED_INFO,
    RX_LLDP_INITIALIZE,
    RX_WAIT_FOR_FRAME,
    DELETE_INFO,
    UPDATE_INFO
}lldp_rxStates_t;

// TX status
#define MAX_TXBUFF_SIZE 500
#define PORT_ID MAC_ADDR

#define txCreditMax     5/**< IEEE 802.1AB 9.2.5.17*/ //default 5
#define msgTxHold       4/**< IEEE 802.1AB 10.5.1  */ //default 4
#define msgTxInterval   30/**< IEEE 802.1AB 10.5.1  */ //default 30s
#define msgFastTx       1/**< IEEE 802.1AB 9.2.5.5 */ //defualt 1 if fast transmot is  needed
#define txFastInit      4/**< IEEE 802.1AB 9.2.5.19*/ //default 4
#define reinitDelay     2/**< IEEE 802.1AB 9.2.5.10*/ //default 2s

/*************************LLDP TX TIMER****************************/
typedef struct
{
    uint16_t txDelay;       /**< IEEE 802.1AB 10.5.3  */ 
    uint16_t txTTR;         /**< IEEE 802.1AB 9.2.2.3 - transmit on expire. */
    uint16_t txShutdownWhile;/**< IEEE 802.1AB 9.2.2.3*/
    uint16_t txDelayWhile;
    bool     txTick;         /**< IEEE 802.1AB 9.2.5.21*/
}lldp_tx_timers_t ;

/**************LLDP TRANSMIT STATE MACHINE****************/

typedef struct
{
    uint64_t statsFramesOutTotal; /**< Defined by IEEE 802.1AB Secion 10.5.2.1 */
}lldp_tx_stats_t ;

  /*This is a per-interface structure.  Part of lldp_port.*/
typedef struct
{
    uint8_t  *frame;            /**< The tx frame buffer */
    uint64_t sendsize;          /**< The size of our tx frame */
    uint8_t  state;             /**< The tx state for this interface */
    bool     localChange;       /**< IEEE 802.1AB var (from where?) */ 
    uint16_t txTTL;             /**< IEEE 802.1AB var (from where?) */
    lldp_tx_timers_t timers;    /**< The lldp tx state machine timers for this interface */
    lldp_tx_stats_t txStats;    /**< The lldp tx statistics for this interface */

    uint8_t txCredit;           /**< IEEE 802.1AB 9.2.5.16*/
    uint8_t txFast;                /**< IEEE 802.1AB 9.2.5.18*/
    bool    txNow;              /**< IEEE 802.1AB 9.2.5.20*/
}lldp_tx_port_t ;


typedef enum
{
    TX_TIMER_INITIALIZE,
    TX_TIMER_IDLE,
    TX_TIMER_EXPIRES,
    TX_TICK,
    SIGNAL_TX,
    TX_FAST_START
}lldp_txTimerStates_t;

typedef enum
{
    TX_LLDP_INITIALIZE,
    TX_IDLE,
    TX_INFO_FRAME,
    TX_SHUTDOWN_FRAME
}lldp_txStates_t;


// Admin status

typedef struct lldp_per_port_t
{
  struct lldp_port *next;
  int socket;        // The socket descriptor for this interface.
  char *if_name;     // The interface name.
  uint32_t if_index; // The interface index.
  uint32_t mtu;      // The interface MTU.
  uint8_t source_mac[6];
  uint8_t source_ipaddr[4];
  lldp_rx_port_t rx;
  lldp_tx_port_t tx;
  bool portEnabled;     /**< IEEE 802.1AB 9.2.4.1 */
  bool newNeighbor;     /**< IEEE 802.1AB 9.2.5.8 */
  bool remoteChanges;   /**< IEEE 802.1AB 9.2.5.11*/
  uint8_t adminStatus;  /**< IEEE 802.1AB 9.2.5.1 */

  uint8_t rxChanges;    /**< IEEE 802.1AB 9.2.5.12*/

  struct lldp_msap *msap_cache;


  // 802.1AB Appendix G flag variables.
  uint8_t  auto_neg_status;
  uint16_t auto_neg_advertized_capabilities;
  uint16_t operational_mau_type;
    // power data
    uint16_t allocatedPower;
    uint16_t desiredPower;

}lldp_per_port_t;

typedef enum
{
    disabled,
    enabledTxOnly,
    enabledRxOnly,
    enabledRxTx
}lldp_admin_status;




#endif // _LLDP_PRIVATE_H

