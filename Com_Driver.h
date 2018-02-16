#ifndef COM_DRIVER_H_
#define COM_DRIVER_H_

#include <asf.h>

struct Com_Driver; // forward declaration

typedef bool             Get_Send_Stop ( void *const /*com_module*/ );
typedef void             Set_Send_Stop ( void *const /*com_module*/, bool const /*new_state*/ );
typedef void             Send_Stop     ( void *const /*com_module*/ );
typedef enum status_code Write_Wait    ( void *const /*com_module*/, uint16_t const /*address*/, uint8_t *const /*byte_array*/, uint16_t const /*cnt_bytes*/ );

/* The struct allows to "overload" the generic Com_Driver functions
 * with specific functions.
 */
struct Com_Driver {
	void*          com_module    ; //< com channel hardware module data structure
	Get_Send_Stop *get_send_stop ;
	Set_Send_Stop *set_send_stop ;
	Send_Stop     *send_stop     ;
	Write_Wait    *write_wait    ;
};

#endif // COM_DRIVER_H_
