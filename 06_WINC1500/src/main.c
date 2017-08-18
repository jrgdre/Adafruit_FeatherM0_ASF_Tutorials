/**	\file	main.c

 	\brief	WINC1500 (WiFi) tutorial application for Adafruit FeatherM0-WiFi (Atmel ATSAMD21G18A) using ASF

 	\license 
 		MIT:	The MIT License (https://opensource.org/licenses/MIT)
 		.
 		Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 		and associated documentation files (the "Software"), to deal in the Software without restriction, 
 		including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 		and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 		subject to the following conditions:
 		.
 		The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 		.
 		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 		INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 		IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 		WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 		THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 	\copyright
 		DIT: 2017; Drechsler Information Technologies; www.drechsler-it.de
 
 	\authors
 		jrgdre: Joerg Drechsler; DIT
 
 	\versions
 		1.0.0: 2017-08-11 jrgdre initial release

 */

/**
* \mainpage FeatherM0-WiFi ASF WINC1500 (WiFi) application documentation
*
* \par WINC1500 (WiFi) tutorial application for Adafruit FeatherM0-WiFi (Atmel SAMD21G18A) using ASF
*
* \par Content
*
* -# Use ASF Wizard with a custom board template to import the modules:
*    - Generic board support (driver)
*    - WINC1500 (WiFi) Host Driver (service)
*    - Standard serial I/O (stdio) (driver)
* -# Include the ASF header files (asf.h)
* -# Include the FatherM0 RS232 declarations (Adafruit_FeatherM0_RS232.h)
* -# Add a LINK to Adafruit_FeatherM0_RS232.c on project level
*/

#include <asf.h>
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "Adafruit_FeatherM0_RS232.h" // include the FatherM0 RS232 declarations

#define MAIN_WLAN_SSID	"Drechsler"
#define MAIN_WLAN_AUTH	M2M_WIFI_SEC_WPA_PSK
#define MAIN_WLAN_PSK	"PLZ10117"

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
"-- DIT Adafruit FeatherM0 ASF Turorials - 03_WINC1500 --"STRING_EOL \
"-- Adafruit FeatherM0-WiFi --"STRING_EOL	\
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

// global variables used in callback functions
static uint8_t	u8ScanResultIdx = 0;	// index of next scan result to read

/**
 * \brief	Convert a WiFi service security type to a string
 *
 * \param[in]	secType		the WiFi security type
 * \returns		Security type string
 */
static const char* wifi_secType_to_string( const tenuM2mSecType secType )
{
	switch( secType ){
		case M2M_WIFI_SEC_INVALID: return "Invalid security type";
		case M2M_WIFI_SEC_OPEN	 : return "not secured";
		case M2M_WIFI_SEC_WPA_PSK: return "WPA/WPA2 personal(PSK)";
		case M2M_WIFI_SEC_WEP	 : return "WEP (40 or 104) OPEN OR SHARED";
		case M2M_WIFI_SEC_802_1X : return "WPA/WPA2 Enterprise, IEEE802.1x user-name/password authentication";
						  default: return "";
	}
}

/**
 * \brief	The WiFi service is done scanning the available APs
 *
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_on_connection_info( void *pvMsg )
{
	tstrM2MConnInfo *pstrM2MConnInfo = (tstrM2MConnInfo *)pvMsg;
			
	printf( "connected to SSID %s MAC %02X:%02X:%02X:%02X:%02X:%02X SEC %s RSSI %d"STRING_EOL,
		pstrM2MConnInfo->acSSID,
		pstrM2MConnInfo->au8MACAddress[0], pstrM2MConnInfo->au8MACAddress[1], pstrM2MConnInfo->au8MACAddress[2],
		pstrM2MConnInfo->au8MACAddress[3], pstrM2MConnInfo->au8MACAddress[4], pstrM2MConnInfo->au8MACAddress[5],
		wifi_secType_to_string( (tenuM2mSecType)pstrM2MConnInfo->u8SecType ),
		pstrM2MConnInfo->s8RSSI
	);
}

/**
 * \brief	WiFi has connected to an AP
 */
static void wifi_on_connected( void )
{
	m2m_wifi_get_connection_info();	
}

/**
 * \brief	WiFi has disconnected from an AP
 */
static void wifi_on_disconnected( void )
{
	printf( "WiFi disconnected"STRING_EOL );
	// try to reconnect
	m2m_wifi_connect( (char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL );
}

/**
 * \brief	The WiFi service is done scanning the available APs
 *
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_on_connection_state_changed( void *pvMsg )
{
	tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;

	switch( (tenuM2mConnState)pstrWifiState->u8CurrState ) {
		case M2M_WIFI_CONNECTED   : wifi_on_connected   (); break;
		case M2M_WIFI_DISCONNECTED: wifi_on_disconnected(); break;
		                   default: break;
	}
}

/**
 * \brief	The WiFi service is done scanning the available APs
 *
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_on_dhcp_ip_obtained( void *pvMsg )
{
	uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
	
	printf( "IP is %u.%u.%u.%u"STRING_EOL, pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3] );
}

/**
 * \brief	The WiFi service is done scanning the available APs
 *
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static sint8 wifi_on_scan_done( void *pvMsg )
{
	tstrM2mScanDone *pstrInfo = (tstrM2mScanDone*)pvMsg;
			
	if( pstrInfo->s8ScanState != M2M_SUCCESS ) {
		printf( "(ERR) Scan fail with error <%d>"STRING_EOL, pstrInfo->s8ScanState );
		return pstrInfo->s8ScanState;
	}
		
	printf( "Number of AP found %d"STRING_EOL, pstrInfo->u8NumofCh );
	
	u8ScanResultIdx = 0;
		
	if( pstrInfo->u8NumofCh >= 1 ) {
		m2m_wifi_req_scan_result( u8ScanResultIdx );
		u8ScanResultIdx ++;
	} 
	else {
		printf( "No AP Found Rescan"STRING_EOL );
		m2m_wifi_request_scan( M2M_WIFI_CH_ALL );
	}
	
	return M2M_SUCCESS;
}

/**
 * \brief	Another WiFi AP result is returned by the WiFi service
 *
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_on_scan_result( void *pvMsg )
{
	tstrM2mWifiscanResult *pstrScanResult = (tstrM2mWifiscanResult*)pvMsg;
	uint8_t                u8NumFoundAPs  = m2m_wifi_get_num_ap_found();
	
	// print the values for the current result
	printf( ">>%02d RI %d SEC %s CH %02d BSSID %02X:%02X:%02X:%02X:%02X:%02X SSID %s"STRING_EOL,
		pstrScanResult->u8index,pstrScanResult->s8rssi,
		wifi_secType_to_string( (tenuM2mSecType)pstrScanResult->u8AuthType ),
		pstrScanResult->u8ch,
		pstrScanResult->au8BSSID[0], pstrScanResult->au8BSSID[1], pstrScanResult->au8BSSID[2],
		pstrScanResult->au8BSSID[3], pstrScanResult->au8BSSID[4], pstrScanResult->au8BSSID[5],
		pstrScanResult->au8SSID
	);
			
	// if not all of the results have been listed yet
	if( u8ScanResultIdx < u8NumFoundAPs ) {
		
		m2m_wifi_req_scan_result( u8ScanResultIdx ); // request the next scan result
		u8ScanResultIdx ++;
		
		return;
	} 

	// if all APs found are listed try to connect to the one defined
	m2m_wifi_connect( (char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL );
}

/**
 * \brief	Callback to get the WiFi status update.
 *
 * \param[in] u8MsgType type of WiFi notification. Possible types are:
 *  - [M2M_WIFI_RESP_CON_STATE_CHANGED](@ref M2M_WIFI_RESP_CON_STATE_CHANGED)
 *  - [M2M_WIFI_REQ_DHCP_CONF](@ref M2M_WIFI_REQ_DHCP_CONF)
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_on_status_changed( uint8_t u8MsgType, void *pvMsg )
{
	switch (u8MsgType) {
		case M2M_WIFI_RESP_SCAN_DONE        : wifi_on_scan_done               ( pvMsg ); break;
		case M2M_WIFI_RESP_SCAN_RESULT      : wifi_on_scan_result             ( pvMsg ); break;
		case M2M_WIFI_RESP_CON_STATE_CHANGED: wifi_on_connection_state_changed( pvMsg ); break;
		case M2M_WIFI_REQ_DHCP_CONF         : wifi_on_dhcp_ip_obtained        ( pvMsg ); break;
		case M2M_WIFI_RESP_CONN_INFO        : wifi_on_connection_info         ( pvMsg ); break;
                                     default: break;
	}
}

/**
 * \brief	Application entry point
 *
 * The application 
 * - Initializes the system
 * - Writes a welcome message
 * - Handles WiFi service messages in a message loop
 */
int main( void )
{
	struct usart_module usart_instance; // an instance of the ASF SERCOM/USART module
	tstrWifiInitParam param;            // WiFi initialization parameters
	int8_t ret;                         // return value
	
	// ============================================
	// board, driver and service initialization [1]
	// ============================================
	
	system_init();                      // board initialization
	system_interrupt_enable_global();   // enable global interrupt system for USART callbacks to work
	
	// =============================================
	// DIT Adafruit_FeatherM0 Library initialization
	// =============================================

	RS232_configure( &usart_instance, 115200 );											// configure USART
	RS232_enable   ( &usart_instance ); //, usart_on_received, usart_on_transmitted );	// enable USART and stdio_serial (if included)

	printf(STRING_HEADER);
	
	// ============================================
	// board, driver and service initialization [2]
	// ============================================

    // Initialize the BSP
	ret = nm_bsp_init();
	if( ret != M2M_SUCCESS ) {
		printf( "main: nm_bsp_init call error!(%d)\r\n", ret );
		while( true ) { // stop processing
		}
	} 

	memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam)); // Initialize WiFi parameters structure

	// Initialize WiFi driver with data and status callbacks. */
	param.pfAppWifiCb = wifi_on_status_changed;
	ret = m2m_wifi_init( &param );
	if( ret != M2M_SUCCESS ) {
		printf( "main: m2m_wifi_init call error!(%d)\r\n", ret );
		while( true ) { // stop processing
		}
	}

	/* Connect to defined AP. */
	ret = m2m_wifi_request_scan( M2M_WIFI_CH_ALL );
	if( ret != M2M_SUCCESS ) {
		printf( "main: m2m_wifi_request_scan call error!(%d)\r\n", ret );
		while( true ) { // stop processing
		}
	}

	// =================
    // application logic
    // =================

	while ( true ) {
		m2m_wifi_handle_events( NULL );
	}
}