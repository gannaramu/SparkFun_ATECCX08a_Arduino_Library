/*
  This is a library written for the ATECCX08A Criptographic Co-Processor (QWIIC).

  Written by Pete Lewis @ SparkFun Electronics, August 5th, 2019

  The IC uses I2C and 1-wire to communicat. This library only supports I2C.

  https://github.com/sparkfun/SparkFun_ATECCX08A_Arduino_Library

  Do you like this library? Help support SparkFun. Buy a board!

  Development environment specifics:
  Arduino IDE 1.8.1

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#if defined(__IMXRT1062__)
// #include <i2c_driver_wire.h>
#include <Wire.h>

#endif
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
#include "i2c_t3.h"
#endif

#define ATECC608A_ADDRESS_DEFAULT 0x35 //7-bit unshifted default I2C Address
// 0x60 on a fresh chip. note, this is software definable

// WORD ADDRESS VALUES
// These are sent in any write sequence to the IC.
// They tell the IC what we are going to do: Reset, Sleep, Idle, Command.
#define WORD_ADDRESS_VALUE_COMMAND 	0x03	// This is the "command" word address, 
//this tells the IC we are going to send a command, and is used for most communications to the IC
#define WORD_ADDRESS_VALUE_IDLE 0x02 // used to enter idle mode

// COMMANDS (aka "opcodes" in the datasheet)
#define COMMAND_OPCODE_INFO 	0x30 // Return device state information.
#define COMMAND_OPCODE_LOCK 	0x17 // Lock configuration and/or Data and OTP zones
#define COMMAND_OPCODE_RANDOM 	0x1B // Create and return a random number (32 bytes of data)
#define COMMAND_OPCODE_READ 	0x02 // Return data at a specific zone and address.
#define COMMAND_OPCODE_WRITE 	0x12 // Return data at a specific zone and address.
#define COMMAND_OPCODE_SHA 		0x47 // Computes a SHA-256 or HMAC/SHA digest for general purpose use by the system.
#define COMMAND_OPCODE_GENKEY 	0x40 // Creates a key (public and/or private) and stores it in a memory key slot
#define COMMAND_OPCODE_NONCE 	0x16 // 
#define COMMAND_OPCODE_SIGN 	0x41 // Create an ECC signature with contents of TempKey and designated key slot
#define COMMAND_OPCODE_VERIFY 	0x45 // takes an ECDSA <R,S> signature and verifies that it is correctly generated from a given message and public key
#define COMMAND_OPCODE_ECDH		0x43 // ECDH pre-master
#define COMMAND_OPCODE_AES_ECB	0x51 // AES_ECB 


// Lock command PARAM1 zone options (aka Mode). more info at table on datasheet page 75
// 		? _ _ _  _ _ _ _ 	Bits 7 verify zone summary, 1 = ignore summary and write to zone!
// 		_ ? _ _  _ _ _ _ 	Bits 6 Unused, must be zero
// 		_ _ ? ?  ? ? _ _ 	Bits 5-2 Slot number (in this example, we use slot 0, so "0 0 0 0")
// 		_ _ _ _  _ _ ? ? 	Bits 1-0 Zone or locktype. 00=Config, 01=Data/OTP, 10=Single Slot in Data, 11=illegal

#define LOCK_MODE_ZONE_CONFIG 			0b10000000
#define LOCK_MODE_ZONE_DATA_AND_OTP 	0b10000001
#define LOCK_MODE_SLOT0					0b10000010

// GenKey command PARAM1 zone options (aka Mode). more info at table on datasheet page 71
#define GENKEY_MODE_PUBLIC 			0b00000000
#define GENKEY_MODE_NEW_PRIVATE 	0b00000100

#define NONCE_MODE_PASSTHROUGH		0b00000011 // Operate in pass-through mode and Write TempKey with NumIn. datasheet pg 79
#define SIGN_MODE_TEMPKEY			0b10000000 // The message to be signed is in TempKey. datasheet pg 85
#define VERIFY_MODE_EXTERNAL		0b00000010 // Use an external public key for verification, pass to command as data post param2, ds pg 89
#define VERIFY_MODE_STORED			0b00000000 // Use an internally stored public key for verification, param2 = keyID, ds pg 89
#define VERIFY_PARAM2_KEYTYPE_ECC 	0x0004 // When verify mode external, param2 should be KeyType, ds pg 89
#define VERIFY_PARAM2_KEYTYPE_NONECC 	0x0007 // When verify mode external, param2 should be KeyType, ds pg 89
#define ECDH_OUTPUT_IN_CLEAR		0b00001100
#define ECDH_OUTPUT_IN_SLOT			0b00000000
#define ECDH_OUTPUT_IN_TEMPKEY		0b00001000
#define AES_ECB_ENCRYPT				0b00000000
#define AES_ECB_DECRYPT				0b00000001
#define WRITE_DATA_32				0b10000010
#define READ_DATA_32				0b10000010

#define ZONE_CONFIG 0x00
#define ZONE_OTP 0x01
#define ZONE_DATA 0x02
#define ZONE_DATA_32 0x82  //32 - byte Data zone read (5.1.6.1)

// #define ZONE_DATA 0x02

#define ADDRESS_CONFIG_READ_BLOCK_0 0x0000 // 00000000 00000000 // param2 (byte 0), address block bits: _ _ _ 0  0 _ _ _ 
#define ADDRESS_CONFIG_READ_BLOCK_1 0x0008 // 00000000 00001000 // param2 (byte 0), address block bits: _ _ _ 0  1 _ _ _ 
#define ADDRESS_CONFIG_READ_BLOCK_2 0x0010 // 00000000 00010000 // param2 (byte 0), address block bits: _ _ _ 1  0 _ _ _ 
#define ADDRESS_CONFIG_READ_BLOCK_3 0x0018 // 00000000 00011000 // param2 (byte 0), address block bits: _ _ _ 1  1 _ _ _

#define ADDRESS_SIGN_CERT_READ_BLOCK_0 0x0060 // 00000000 00000000 // param2 (byte 0), address block bits: _ _ _ 0  0 _ _ _
#define ADDRESS_SIGN_CERT_READ_BLOCK_1 0x0160 // 00000000 00001000 // param2 (byte 0), address block bits: _ _ _ 0  1 _ _ _
#define ADDRESS_SIGN_CERT_READ_BLOCK_2 0x0260 // 00000000 00010000 // param2 (byte 0), address block bits: _ _ _ 1  0 _ _ _
#define ADDRESS_SIGN_CERT_READ_BLOCK_3 0x0261 // 00000000 00011000 // param2 (byte 0), address block bits: _ _ _ 1  1 _ _ _

#define ADDRESS_SIGN_PUB_KEY_READ_BLOCK_0 0x0058 // 00000000 00000000 // param2 (byte 0), address block bits: _ _ _ 0  0 _ _ _
#define ADDRESS_SIGN_PUB_KEY_READ_BLOCK_1 0x0158 // 00000000 00001000 // param2 (byte 0), address block bits: _ _ _ 0  1 _ _ _
#define ADDRESS_SIGN_PUB_KEY_READ_BLOCK_2 0x0258 // 00000000 00010000 // param2 (byte 0), address block bits: _ _ _ 1  0 _ _ _
#define ADDRESS_SIGN_PUB_KEY_READ_BLOCK_3 0x0259 // 00000000 00011000 // param2 (byte 0), address block bits: _ _ _ 1  1 _ _ _

#define ADDRESS_DATA_READ_SLOT10_BLOCK_0	0x0050
#define ADDRESS_DATA_READ_SLOT10_BLOCK_1	0x0150
#define ADDRESS_DATA_READ_SLOT11_BLOCK_0 	0x080
#define ADDRESS_DATA_READ_SLOT11_BLOCK_1 	0x0350
class ATECCX08A {
  public:
    //By default use Wire, standard I2C speed, and the default ADS1015 address
	#if defined(__IMXRT1062__)
	boolean begin(uint8_t i2caddr = ATECC608A_ADDRESS_DEFAULT, TwoWire &wirePort = Wire);
	#endif
	#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	boolean begin(uint8_t i2caddr = ATECC608A_ADDRESS_DEFAULT, i2c_t3 &wirePort = Wire);
	#endif
	
	uint8_t inputBuffer[128]; // used to store messages received from the IC as they come in
	uint8_t configZone[128]; // used to store configuration zone bytes read from device EEPROM
	uint8_t storedPublicKey[64];
	uint8_t DeviceCertificate[72];
	uint8_t SignerCertificate[72];
	uint8_t SignerPublicKey[72];
	uint8_t SignerPublicKeyX[36];
	uint8_t SignerPublicKeyY[36];

	uint8_t revisionNumber[5]; // used to store the complete revision number, pulled from configZone[4-7]
	uint8_t serialNumber[9]; // used to store the complete Serial number, pulled from configZone[0-3] and configZone[8-12]
	boolean configLockStatus; // pulled from configZone[87], then set according to status (0x55=UNlocked, 0x00=Locked)
	boolean dataOTPLockStatus; // pulled from configZone[86], then set according to status (0x55=UNlocked, 0x00=Locked)
	boolean slot0LockStatus; // pulled from configZone[88], then set according to slot (bit 0) status
	
	uint8_t publicKey64Bytes[64]; // used to store the public key returned when you (1) create a keypair, or (2) read a public key
	uint8_t signature[64];
	uint8_t ECDH_secret[32];
	uint8_t AES_buffer[16];
	boolean receiveResponseData(uint8_t length = 0, boolean debug = false);
	boolean checkCount(boolean debug = false);
	boolean checkCrc(boolean debug = false);
	uint8_t countGlobal = 0; // used to add up all the bytes on a long message. Important to reset before each new receiveMessageData();
	void cleanInputBuffer();
	
	boolean wakeUp();
	void idleMode();
	boolean getInfo();
	boolean writeConfigSparkFun();
	boolean lockConfig(); // note, this PERMINANTLY disables changes to config zone - including changing the I2C address!
	boolean lockDataAndOTP();
	boolean lockDataSlot0();
	boolean lockDataSlot(int slot);
	boolean lock(uint8_t zone);
	
	// Random array and fuctions
	uint8_t random32Bytes[32]; // used to store the complete data return (32 bytes) when we ask for a random number from chip.
	boolean updateRandom32Bytes(bool debug = false);
	uint8_t getRandomByte(bool debug = false);
	int getRandomInt(bool debug = false);
	long getRandomLong(bool debug = false);
	long random(long max);
	long random(long min, long max);
	
	uint8_t crc[2] = {0, 0};
	void atca_calculate_crc(uint8_t length, uint8_t *data);	
	
	// Key functions
	boolean createNewKeyPair(uint16_t slot = 0x0000, bool debug = false);
	boolean generatePublicKey(uint16_t slot = 0x0000, bool debug = false);
	
	boolean createSignature(uint8_t *data, uint16_t slot = 0x0000, bool debug = false); 
	boolean loadTempKey(uint8_t *data, bool debug = false);  // load 32 bytes of data into tempKey (a temporary memory spot in the IC)
	boolean signTempKey(uint16_t slot = 0x0000, bool debug = false); // create signature using contents of TempKey and PRIVATE KEY in slot
	boolean verifySignature(uint8_t *message, uint8_t *signature, uint8_t *publicKey, bool debug = false); // external ECC publicKey only

	boolean read(uint8_t zone, uint16_t address, uint8_t length, bool debug = false);
	boolean write(uint8_t zone, uint16_t address, uint8_t *data, uint8_t length_of_data);

	boolean readConfigZone(bool debug = false);
	
	
	boolean readSignerCertificate(boolean debug = false);
	boolean readSignerPublicKey(boolean debug = false);

	boolean sendCommand(uint8_t command_opcode, uint8_t param1, uint16_t param2, uint8_t *data = NULL, size_t length_of_data = 0);
	boolean ECDH(uint8_t *data, uint8_t mode, uint16_t slot, bool debug = false);
	boolean AES_ECB_encrypt(uint8_t *data, uint16_t slot = 0xFFFF, bool debug = false );
	boolean AES_ECB_decrypt(uint8_t *data, uint16_t slot = 0xFFFF, bool debug = false );
	boolean writeProvisionConfig();
	boolean loadPublicKey(uint8_t *data, bool debug = false);
	boolean readPublicKey(bool debug = false);
  private:

	#if defined(__IMXRT1062__)
	TwoWire *_i2cPort;
	#endif
	#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	i2c_t3 *_i2cPort;
	#endif

	uint8_t _i2caddr;
	
};


