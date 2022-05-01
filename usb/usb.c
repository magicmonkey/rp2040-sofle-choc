#include "usb.h"

#include "tusb.h"
#include "usb_descriptors.h"
#include "bsp/board.h"
#include "../keyboard/keyboard.h"

uint8_t keyToHid[30] = {
	HID_KEY_A,   HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5,
	HID_KEY_TAB, HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T,
	HID_KEY_A,   HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G,
	HID_KEY_A,   HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B,
	HID_KEY_A,   HID_KEY_A, HID_KEY_A, HID_KEY_A, HID_KEY_A, HID_KEY_A,
};

void usb_init() {
	tusb_init();
}

void usb_task() {
	tud_task();
	hid_task();
}

void translateKeysToHid(uint8_t *srcKeys, uint8_t *dstKeys) {
	for (int i=0; i<6; i++) {
		dstKeys[i] = keyToHid[srcKeys[i]];
	}
	return;
}

static void send_hid_report() {
	// skip if hid is not ready yet
	if ( !tud_hid_ready() ) return;

	// use to avoid send multiple consecutive zero report for keyboard
	static bool has_keyboard_key = false;

	bool somethingPressed = is_something_pressed();

	uint8_t keysHid[6];
	translateKeysToHid(keyboard_get_keys_pressed(), keysHid);

	if (somethingPressed) {
		tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keysHid);
		has_keyboard_key = true;
	} else {
		// send empty key report if previously has key pressed
		if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
		has_keyboard_key = false;
	}

}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1;

  if (next_report_id < REPORT_ID_COUNT) {
    send_hid_report();
  }
}

void hid_task()
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  bool somethingPressed = is_something_pressed();

  if (somethingPressed) {
	// Remote wakeup
	if (tud_suspended()) {
		// Wake up host if we are in suspend mode
		// and REMOTE_WAKEUP feature is enabled by host
		tud_remote_wakeup();
	}else {
		// Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
		send_hid_report();
	}
  }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

	  // Stuff here, like change the caps lock light
    }
  }
}

