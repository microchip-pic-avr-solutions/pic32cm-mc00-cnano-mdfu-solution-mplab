#include <stdint.h>

#define EXECUTION_IMAGE_ID 0x00000000U

// NOTE: The top 2 bytes of this object are unused.
volatile const uint32_t
applicationSlotId __attribute__((used, section("application_slot_id"), space(prog), address(0x10FEC))) = EXECUTION_IMAGE_ID;

volatile const uint32_t
applicationVersion __attribute__((used, section("application_version"), space(prog), address(0x10FF0))) = 0x00000100U;

volatile const uint32_t
verificationEndAddress __attribute__((used, section("application_verify_end"), space(prog), address(0x10FF4))) = 0x00010FFBU;

volatile const uint32_t
verificationStartAddress __attribute__((used, section("application_verify_start"), space(prog), address(0x10FF8))) = 0x00002000U;

volatile const uint32_t
applicationHash __attribute__((used, section("crc_start_address"), space(prog), address(0x10FFC))) = 0xFFFFFFFFU;